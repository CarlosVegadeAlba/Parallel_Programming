import matplotlib.pyplot as plt

# Data
road_usa = {
    0: 0.960,
    50: 0.985,
    100: 0.979,
    150: 0.954,
    200: 0.955,
    300: 0.940
}

delaunay_n24 = {
    0: 0.922,
    50: 0.929,
    100: 0.946,
    150: 0.887,
    200: 0.848,
    300: 0.762
}

hugebubbles_00020 = {
    0: 0.970,
    50: 0.985,
    100: 0.963,
    150: 0.955,
    200: 0.955,
    300: 0.949
}

rgg_n_2_22_s0 = {
    0: 0.998,
    50: 0.989,
    100: 0.958,
    150: 0.886,
    200: 0.847,
    300: 0.673
}

# Plotting
plt.figure(figsize=(10, 6))

plt.plot(list(road_usa.keys()), list(road_usa.values()), marker='o', label='Road_usa')
plt.plot(list(delaunay_n24.keys()), list(delaunay_n24.values()), marker='o', label='Delaunay_n24')
plt.plot(list(hugebubbles_00020.keys()), list(hugebubbles_00020.values()), marker='o', label='Hugebubbles-00020')
plt.plot(list(rgg_n_2_22_s0.keys()), list(rgg_n_2_22_s0.values()), marker='o', label='Rgg_n_2_22_s0')

plt.title('Number of Sequential Rounds vs Average Mean Speedup')
plt.xlabel('Number of Sequential Rounds')
plt.ylabel('Average Mean Speedup')
plt.legend()
plt.grid(True)
plt.show()
