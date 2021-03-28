import numpy as np
import tensorflow as tf
import tensorflow_probability as tfp
from typing import List, Optional, Set


class VertexToEdge:
    edge_index: int
    index_in_edge: int

    def __init__(self, edge_index: int, index_in_edge: int) -> None:
        assert edge_index >= 0
        assert index_in_edge == 0 or index_in_edge == 1
        self.edge_index = edge_index
        self.index_in_edge = index_in_edge

    def __hash__(self) -> int:
        return (self.edge_index, self.index_in_edge).__hash__()

    def __eq__(self, o: object) -> bool:
        if not isinstance(o, VertexToEdge):
            return False
        return self.edge_index == o.edge_index and self.index_in_edge == o.index_in_edge


class Mesh:
    faces: np.ndarry
    vertices: np.ndarry
    edges: np.ndarry
    edge_neighbors: np.ndarray
    edge_lookup: np.ndarray
    vertex_to_edges: List[Optional[Set[VertexToEdge]]]
    vertex_mask: np.ndarray
    edge_mask: np.ndarray
    max_vertex_degree: int
    edge_num: int
    vertex_to_edges_tensor: tf.Tensor
    vertex_to_degree: tf.Tensor

    def __init__(self, vertices: np.ndarray, faces: np.ndarray) -> None:
        assert isinstance(vertices, np.ndarray)
        assert vertices.shape[1] == 3
        assert isinstance(faces, np.ndarray)
        assert faces.shape[1] == 3
        self.vertices = vertices
        self.faces = faces
        self.build()
        self.vertex_mask = np.ones(self.vertices.shape[0], dtype=np.bool)
        self.edge_mask = np.ones(self.edges.shape[0], dtype=np.bool)
        self.edge_num = self.edges.shape[0]

    def build(self):
        self.vertex_to_edges = [set() for _ in range(self.vertices.shape[0])]
        edge_to_key = {}  # unique edge index
        key_to_edge = []  # smaller vertex index and bigger vertex index
        edge_neighbors: List[List[int]] = []
        edge_neighbors_num: List[int] = []
        edge_lookup: List[List[int]] = []

        for face in self.faces:
            face_edges = [
                tuple(sorted([face[i], face[(i+1) % 3]])) for i in range(3)
            ]

            for edge_vertices in face_edges:
                if edge_vertices not in edge_to_key:
                    edge_key = len(edge_to_key)
                    edge_to_key[edge_vertices] = edge_key
                    key_to_edge.append(edge_vertices)

                    edge_neighbors.append([None, None, None, None])
                    edge_lookup.append([None, None, None, None])
                    edge_neighbors_num.append(0)

                    v0, v1 = edge_vertices
                    self.vertex_to_edges[v0].add(VertexToEdge(edge_key, 0))
                    self.vertex_to_edges[v1].add(VertexToEdge(edge_key, 1))

            for edge_index, edge_vertices in enumerate(face_edges):
                neighbor_0_key = edge_to_key[face_edges[(edge_index+1) % 3]]
                neighbor_1_key = edge_to_key[face_edges[(edge_index+2) % 3]]

                edge_key = edge_to_key[edge_vertices]
                neighbors_num = edge_neighbors_num[edge_key]
                edge_neighbors[edge_key][neighbors_num+0] = neighbor_0_key
                edge_neighbors[edge_key][neighbors_num+1] = neighbor_1_key
                edge_neighbors_num[edge_key] += 2

            for edge_index, edge_vertices in enumerate(face_edges):
                neighbor_0_key = edge_to_key[face_edges[(edge_index+1) % 3]]
                neighbor_1_key = edge_to_key[face_edges[(edge_index+2) % 3]]

                neighbors_num_0 = edge_neighbors_num[neighbor_0_key]
                neighbors_num_1 = edge_neighbors_num[neighbor_1_key]

                edge_key = edge_to_key[edge_vertices]
                neighbors_num = edge_neighbors_num[edge_key]
                edge_lookup[edge_key][neighbors_num-2] = neighbors_num_0-1
                edge_lookup[edge_key][neighbors_num-1] = neighbors_num_1-2

        self.edges = np.array(key_to_edge, dtype=np.int32)
        self.edge_neighbors = np.array(edge_neighbors, dtype=np.int64)
        self.edge_lookup = np.array(edge_lookup, dtype=np.int64)

        self.max_vertex_degree = max(
            [len(ec) for ec in self.vertex_to_edges]
        )

        self.vertex_to_edges_tensor = []
        self.vertex_to_degree = []
        for ecs in self.vertex_to_edges:
            vertex_info = [(ec.edge_index, ec.index_in_edge)for ec in ecs]

            num_e = self.edges.shape[0]

            vertex_info.extend(
                [num_e, 0] for _ in range(self.max_vertex_degree-len(ecs))
            )

            self.vertex_to_edges_tensor.extend(vertex_info)
            self.vertex_to_degree.append(len(ecs))
        self.vertex_to_edges_tensor = tf.convert_to_tensor(
            self.vertex_to_edges_tensor
        )
        self.vertex_to_degree = tf.convert_to_tensor(
            self.vertex_to_degree, dtype=tf.float32
        )

    def collapse_masked_elements(self) -> None:
        self.vertices = self.vertices[self.vertex_mask]
        new_vertex_indices = np.zeros_like(self.vertex_mask, dtype=np.int32)
        new_vertex_indices[self.vertex_mask] = np.arange(
            self.vertices.shape[0]
        )

        self.edges = new_vertex_indices[self.edges[self.edge_mask]]
        new_edge_indices = np.zeros_like(self.edge_mask, dtype=np.int32)
        new_edge_indices[self.edge_mask] = np.arange(self.edges.shape[0])

        self.edge_neighbors = new_edge_indices[self.edge_neighbors[self.edge_mask]]

        new_vertex_to_edges = []
        for vertex2edges in self.vertex_to_edges:
            if vertex2edges is None:
                continue
            new_VertexToEdge = {
                VertexToEdge(
                    new_edge_indices[old.edge_index], old.index_in_edge)
                for old in vertex2edges
            }
            new_vertex_to_edges.append(new_VertexToEdge)
        self.vertex_to_edges = new_vertex_to_edges

        self.edge_lookup = self.edge_lookup[self.edge_mask]

        self.vertex_mask = np.ones(self.vertices.shape[0], dtype=np.bool)
        self.edge_mask = np.ones(self.edges.shape[0], dtype=np.bool)


    def generate_face_areas_normals(self, vertices):
        faces = self.faces
        edge_vectors_1 = tf.gather(vertices, faces[:, 1]) - tf.gather(
            vertices, faces[:, 0]
        )
        edge_vectors_2 = tf.gather(vertices, faces[:, 2]) - tf.gather(
            vertices, faces[:, 1]
        )
        edge_cross = tf.linalg.cross(edge_vectors_1, edge_vectors_2)

        edge_cross_mag = tf.norm(edge_cross, axis=1)

        face_unit_normals = edge_cross / edge_cross_mag[:, None]

        face_areas = 0.5 * edge_cross_mag

        return face_unit_normals, face_areas

    def sample_surface(self, vertices, count):
        # ARGS:
        #     verticies: tensor of shape (num_verticies, 3) XYZ coordinate associated with each vertex
        #     self.faces: np array (num_faces, 3) set of three row indexesassociated with each face
        #     self.face_areas: np array (num_faces) hlding the area of each face
        #     self.face_unit_normals: np array (num_faces, 3) representing the XYZ vector of the unti normal for the face
        #     count: number of points to sample

        # Uses:
        # https://mathworld.wolfram.com/TrianglePointPicking.html

        # returns:
        #     sample_points: tf (count, 3) XYZ coordinate for randomly sampled points accross the mesh
        #     sample_normals: tf (count, 3) XYZ norms for the selected points

        faces = self.faces
        face_unit_normals, face_areas = self.generate_face_areas_normals(
            vertices
        )
        # normalize the face areas by the total area
        total_face_area = tf.math.reduce_sum(face_areas)
        face_areas = face_areas / total_face_area

        # Creates a probability distribution from the face areas
        face_distribution = tfp.distributions.Categorical(probs=face_areas)

        # samples the distribution count number of times, then gets the face row value for the relevant face
        face_to_sample = face_distribution.sample(count)
        face_index = face_to_sample  # (count)
        face_to_sample = tf.gather(faces, face_to_sample)  # (count, 3)

        # sets XYZ "origins" for each triangle as the 0th index vertex (count, 3)
        origin = tf.gather(
            vertices,
            face_to_sample[:, 0],
        )

        # defines the two edges that, with the origin, define the triangle (count, 3)
        edge_1 = tf.gather(vertices, face_to_sample[:, 1]) - origin
        edge_2 = tf.gather(vertices, face_to_sample[:, 2]) - origin

        # stacks the two edge matricies together, dim (count, 2, 3)
        edges = tf.stack([edge_1, edge_2], axis=1)

        # computes two values between 0 and 1 that will scale the edge vectors and then summed to compute the points
        # dim (count, 2)
        edge_weights = tf.random.uniform(shape=(count, 2))

        # some points would be outside the triangle. if the sum is less than one then it is insde and thus
        # outside_triangle is true. dim (count)
        outside_triange = tf.math.reduce_sum(edge_weights, axis=1) > 1.0

        # remaps the points that are outside the trianle inside of the triangle
        edge_weights = edge_weights - tf.expand_dims(
            tf.cast(outside_triange, dtype=tf.float32), axis=1
        )
        edge_weights = tf.math.abs(edge_weights)

        # computes a sample vector as the weighted sum of the edge vectors using the random weights from above
        # dim (count, 3)
        sample_vector = tf.math.reduce_sum(
            edges * tf.expand_dims(edge_weights, axis=2), axis=1
        )

        # sample points are the displacement vector plus the origin
        # dim (count, 3)
        sample_points = sample_vector + origin

        # gather the normal for each point from the face it was sampled from
        sample_normals = tf.gather(face_unit_normals, face_index)

        return sample_points, sample_normals


class MeshCherker:
    def __init__(self, mesh: Mesh) -> None:
        self.mesh = mesh
    
    def _check_vertices(self) -> bool:
        if self.mesh.vertices.shape[1] != 3:
            return False
        vertices_seen_in_faces = set()
        for a, b, c in self.mesh.faces:
            vertices_seen_in_faces.add(a)
            vertices_seen_in_faces.add(b)
            vertices_seen_in_faces.add(c)
        vertices_seen_in_edges = set()
        for edge_key, edge in enumerate(self.mesh.edges):
            if not self.mesh.edge_mask[edge_key]:
                continue
            vertices_seen_in_edges.add(edge[0])
            vertices_seen_in_edges.add(edge[1])
        for vertex_key, xyz in enumerate(self.mesh.vertices):
            if not self.mesh.vertex_mask[vertex_key]:
                continue
            if np.isnan(xyz).any():
                return False
            if vertex_key not in vertices_seen_in_edges:
                return False
            if vertex_key not in vertices_seen_in_faces:
                return False
            if self.mesh.vertex_to_edges[vertex_key] is None:
                return False
        return True
    
    def _check_edges(self) -> bool:
        if self.mesh.edges.shape[1] != 2:
            return False
        for edge_key, edge in enumerate(self.mesh.edges):
            if not self.mesh.edge_mask[edge_key]:
                continue
            a = not self.mesh.vertex_mask[edge[0]]
            b = not self.mesh.vertex_mask[edge[1]]
            if a or b:
                return False
        return True

    def _check_edge_nerghbors_and_edge_lookup(self) -> bool:
        edge_num = self.mesh.edges.shape[0]
        if self.mesh.edge_neighbors.shape != (edge_num, 4):
            return False
        if self.mesh.edge_lookup.shape != (edge_num, 4):
            return False
        for edge_key, neighbors in enumerate(self.mesh.edge_neighbors):
            if not self.mesh.edge_mask[edge_key]:
                continue

            lookup = self.mesh.edge_lookup[edge_key, :]
            for neighbor_key, neighbor in enumerate(neighbors):
                expected_edge_key = self.mesh.edge_neighbors[
                    neighbor, lookup[neighbor_key]
                ]
                if edge_key != expected_edge_key:
                    return False

            neighbors = self.mesh.edge_neighbors[edge_key, :]
            for neighbor_key in neighbors:
                if not self.mesh.edge_mask(neighbor_key):
                    return False
        return True

    def _check_vertex_to_edges(self)->bool:
        for vertex_key, vertex2edges in enumerate(self.mesh.vertex_to_edges):
            if vertex2edges is None:
                if not self.mesh.vertex_mask(vertex_key):
                    return False
                continue
            for vertex2edge in vertex2edges:
                edge_index = vertex2edge.edge_index
                index_in_edge = vertex2edge.index_in_edge
                if not self.mesh.edge_mask(edge_index):
                    return False
                if self.mesh.edges[edge_index, index_in_edge] != vertex_key:
                    return False
        return True
    
    def _check_edge_mask(self)->bool:
        e_num = self.mesh.edges.shape[0]
        if self.mesh.edge_mask.shape != (e_num,):
            return False
        if self.mesh.edge_num != np.sum(self.mesh.edge_mask):
            return False
        return True
    
    def _check_vertex_mask(self)->bool:
        v_num = self.mesh.vertices.shape[0]
        if self.mesh.vertex_mask != (v_num,):
            return False
        return True
    
    def check_validity(self)-> bool:
        return (
            True
            and self._check_vertices()
            and self._check_edges()
            and self._check_edge_nerghbors_and_edge_lookup()
            and self._check_vertex_to_edges()
            and self._check_edge_mask()
            and self._check_vertex_mask()
        )
    

def feature_valid(mesh: Mesh, features: tf.Tensor) -> bool:
    # Check whether a feature tensor is compatible with the specified mesh.
    # A feature tensor's shape should be (feature_length, num_edges).
    return tf.rank(features) == 2 and features.shape[0] == mesh.edges.shape[0]
