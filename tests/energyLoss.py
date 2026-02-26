RayEnergy = 1.0


#            Air  Concrete  Idk?
materiaux = [0.01, 0.11,    0.34]


print(f"Ray init: {RayEnergy}")
for i in range(10):
    if RayEnergy < 0.05:
        print("Ray killed")
        break
    RayEnergy -= materiaux[2]*RayEnergy
    print(f"Current Ray: {RayEnergy}")
     