import json

DEFAULTS = {
    # The point cloud that's fitted.
    # "point_cloud": "data/point_clouds/elephant.pwn",
    "point_cloud": "data/airplane_0003.xyz",
    # The number of times remeshing/subdivision happens.
    "num_subdivisions": 6,
    # The number of iterations between each remeshing/subdivision.
    "num_iterations": 1000,
    # Each subdivision multiplies the number of faces by this.
    "subdivision_multiplier": 1.5,
    # The maximum number of faces that subdivision is allowed to yield.
    "max_num_faces": 10000,
    # The initial number of faces used for optimization.
    "initial_num_faces": 1000,
    # An optional initial mesh.
    "initial_mesh": None,
    # The folder where the results are saved.
    "save_location": "data/results/airplane_0003    ",   
    # how often to run beamgap loss if -1 then no beam gap loss
    "beamgap_modulo": -1,
    # how often to save objs
    "obj_save_modulo": 5,
    # range to lineralyinterp between when computing samples
    "min_num_samples": 10000,
    "max_num_samples": 16000,
    "pooling": [None, None, None, None, None, None],
}


def load_options(argv: list):
    if len(argv) == 1:
        return DEFAULTS
    if len(argv) != 2:
        raise Exception(
            "The only (optional) argument should be the settings JSON file."
        )
    with open(argv[1], "r") as f:
        options = json.load(f)

    for key in options:
        if key not in DEFAULTS:
            raise Exception(f'Unknown setting "{key}"')
    return {**DEFAULTS, **options}
