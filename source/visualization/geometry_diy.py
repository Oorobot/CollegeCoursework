class Geometry_DIY():
    id: int
    path: str
    name: str
    def __init__(self, id=0, name="", geometry=None) -> None:
        self.id = id
        self.name = name
        self.geometry = geometry

    def __str__(self) -> str:
        return f"[{self.__class__.__name__}: id={self.id}, name={self.name}, geometry={self.geometry}]"
    
    def __eq__(self, o: object) -> bool:
        if not isinstance(o, Geometry_DIY):
            return False
        return self.name == o.name and self.id == o.id


print(Geometry_DIY(1,"zero"))
list1 = []
list1.append(Geometry_DIY(1,"zero"))
list1.append(Geometry_DIY(2,"zero"))
print(list1)
