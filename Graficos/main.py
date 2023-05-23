import matplotlib.pyplot as plt

# Médias dos tempos
bfs = [1.026, 1.067, 1.035, 1.056, 1.029, 1.027, 1.070, 1.054, 1.028, 1.031, 1.021, 1.026,
       1.027, 1.028, 1.058, 1.040, 2.759, 1.017, 1.024, 1.018, 1.044, 1.028, 1.022, 1.027, 1.143]
sumBFS = sum(bfs)/len(bfs)

idfs = [0.984, 0.975, 0.972, 0.982, 0.981, 1.031, 1.078, 0.974, 0.984, 0.973, 1.041, 0.972,
        0.980, 0.989, 0.967, 0.971, 0.974, 1.032, 0.973, 0.975, 0.0986, 0.990, 0.991, 0.970, 0.971]
sumIDFS = sum(idfs)/len(idfs)

aEstrela = [1.159, 1.249, 1.175, 1.175, 1.157, 1.158, 1.189, 1.174, 1.191, 1.184, 1.164, 1.166,
            1.166, 1.163, 1.173, 1.177, 1.201, 1.172, 1.157, 1.191, 1.174, 1.161, 1.171, 1.162, 1.168, 1.191]
sumAEstrela = sum(aEstrela)/len(aEstrela)

encosta = [1.368, 1.341, 1.372, 1.319, 1.323, 1.322, 1.351, 1.372, 1.327, 1.323, 1.348, 1.344,
           1.353, 1.332, 1.320, 1.341, 1.352, 1.401, 1.358, 1.358, 1.323, 1.322, 1.324, 1.346, 1.322, 1.415]
sumEncosta = sum(encosta)/len(encosta)

# Dados para o gráfico
x = ['BFS', 'IDFS', 'A*', 'Hill Climbing']
y = [sumBFS, sumIDFS, sumAEstrela, sumEncosta]

# Criar o gráfico de barras
plt.bar(x, y)

# Adicionar rótulos e título
plt.xlabel('Buscas')
plt.ylabel('Tempos (seg)')
plt.title('Média geral labirinto')

# Exibir o gráfico
plt.show()
