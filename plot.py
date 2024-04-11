import subprocess
import matplotlib.pyplot as plt
import os
import random
import time

# Récupérer la liste des fichiers exécutables dans le répertoire "build"
executables = [f for f in os.listdir("build") if os.path.isfile(
    os.path.join("build", f)) and os.access(os.path.join("build", f), os.X_OK)]

executables = [exe for exe in executables if "-pthread" not in exe]

execution_times_thread = {}
execution_times_pthread = {}

num_arguments = int(
    input("Entrez le nombre d'arguments à tester (par défaut 5): ") or 5)

for exe in executables:
    arguments_to_test = random.sample(range(1, 20), num_arguments)
    second_arg = 5
    for arg in arguments_to_test:
        try:
            if exe.startswith(("31", "32", "33")):
                start_time = time.time()  # Temps de début de l'exécution
                subprocess.run(["./build/" + exe, str(arg), str(second_arg)],
                               stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL, check=True)
                end_time = time.time()  # Temps de fin de l'exécution
                execution_time_thread = end_time - start_time  # Calcul de la durée d'exécution

                start_time = time.time()  # Temps de début de l'exécution
                subprocess.run(["./build/" + exe + "-pthread", str(arg), str(second_arg)],
                               stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL, check=True)
                end_time = time.time()  # Temps de fin de l'exécution
                execution_time_pthread = end_time - start_time  # Calcul de la durée d'exécution
            else:
                start_time = time.time()  # Temps de début de l'exécution
                subprocess.run(["./build/" + exe, str(arg)],
                               stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL, check=True)
                end_time = time.time()  # Temps de fin de l'exécution
                execution_time_thread = end_time - start_time  # Calcul de la durée d'exécution

                start_time = time.time()  # Temps de début de l'exécution
                subprocess.run(["./build/" + exe + "-pthread", str(arg)],
                               stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL, check=True)
                end_time = time.time()  # Temps de fin de l'exécution
                execution_time_pthread = end_time - start_time  # Calcul de la durée d'exécution

            execution_times_thread.setdefault(
                exe, []).append((arg, execution_time_thread))
            execution_times_pthread.setdefault(
                exe, []).append((arg, execution_time_pthread))
        except subprocess.CalledProcessError as e:
            print(
                f"Erreur lors de l'exécution de '{exe}' avec l'argument '{arg}': {e.output}")

plt.figure(figsize=(16, 8))

for exe, times in execution_times_thread.items():
    args, durations = zip(*times)
    args_sorted, durations_sorted = zip(*sorted(zip(args, durations)))
    plt.plot(args_sorted, durations_sorted, marker='o', label=exe + "_thread")

for exe, times in execution_times_pthread.items():
    args, durations = zip(*times)
    args_sorted, durations_sorted = zip(*sorted(zip(args, durations)))
    plt.plot(args_sorted, durations_sorted, marker='o', label=exe + "_pthread")

plt.xlabel('Argument')
plt.ylabel('Durée d\'exécution (s)')
plt.title('Durée d\'exécution des programmes avec différents arguments')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
