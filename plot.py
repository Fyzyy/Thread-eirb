import subprocess
import matplotlib.pyplot as plt
import os
import time

# Create a folder to save plots if it doesn't exist
if not os.path.exists("plots"):
    os.makedirs("plots")

# Récupérer la liste des fichiers exécutables dans le répertoire "build"
executables = [f for f in os.listdir("build") if os.path.isfile(
    os.path.join("build", f)) and os.access(os.path.join("build", f), os.X_OK)]

executables = [exe for exe in executables if "-pthread" not in exe]

execution_times_thread = {}
execution_times_pthread = {}

num_arguments = int(
    input("Entrez le nombre d'arguments à tester (par défaut 5): ") or 5)

for exe in executables:
    for arg in range(num_arguments + 1):
        try:
            second_arg = 5
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

                execution_times_thread.setdefault(
                    exe, []).append(execution_time_thread)
                execution_times_pthread.setdefault(
                    exe, []).append(execution_time_pthread)
            elif exe.startswith(("81", "01", "02", "03", "11")):
                continue
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
                    exe, []).append(execution_time_thread)
                execution_times_pthread.setdefault(
                    exe, []).append(execution_time_pthread)
        except subprocess.CalledProcessError as e:
            print(
                f"Erreur lors de l'exécution de '{exe}' avec l'argument '{arg}': {e.output}")

for exe, durations_thread in execution_times_thread.items():
    durations_pthread = execution_times_pthread.get(exe, [])
    num_threads = len(durations_thread)
    num_pthreads = len(durations_pthread)
    min_len = min(num_threads, num_pthreads)
    plt.plot(range(min_len),
             durations_thread[:min_len], '-bo', label=exe+"_thread")
    plt.plot(range(min_len),
             durations_pthread[:min_len], '-ro', label=exe+"_pthread")
    plt.xlabel('Nombre de threads')
    plt.ylabel('Durée d\'exécution (s)')
    plt.title('Durée d\'exécution des programmes avec différents arguments')
    plt.legend()
    plt.grid(True)
    plt.savefig(f"plots/{exe}.png")
    plt.close()
    plt.show()

print("Done")