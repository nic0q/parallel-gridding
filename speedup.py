import numpy as np
from matplotlib import pyplot as plt

t_p = [3.14326,
       4.15092,
       4.1311,
       4.25886,
       4.29552,
       4.46537,
       4.45734,
       4.42189,
       4.34559,
       4.41952,
       4.34371,
       4.5731,
       4.48062,
       4.51469
       ]

t_s = 2.92978
speedup = []
x = []
for i in range(0, len(t_p)):
    x = np.append(x, i+1)
    speedup = np.append(speedup, t_s/t_p[i])
    print(x[i])
plt.xlabel("N° de tareas")
plt.ylabel("Speedup")
plt.title("Comportamiento speedup / método matrices compartidas")
plt.plot(x, speedup)
plt.grid(color='lime', linestyle='--', linewidth=0.5)
plt.show()
