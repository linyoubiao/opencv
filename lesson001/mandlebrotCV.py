"""
PyQt AND OpenCV
By LiNYoUBiAO
2020/4/19 10:53
"""
import numpy as np
import cv2 as cv


def mandelbrot_set(xmin, xmax, ymin, ymax, xn, yn, maxiter, horizon=2.0):
    X = np.linspace(xmin, xmax, xn)
    Y = np.linspace(ymin, ymax, yn)
    C = X + Y[:, None] * 1j
    N = np.zeros_like(C, dtype=int)
    Z = np.zeros_like(C)
    for n in range(maxiter):
        I = np.less(abs(Z), horizon)
        N[I] = n
        Z[I] = Z[I] ** 2 + C[I]
    N[N == maxiter - 1] = 0
    return Z, N


if __name__ == '__main__':
    import matplotlib.pyplot as plt

    xmin, xmax, xn = -2.25, +0.75, 1500
    ymin, ymax, yn = -1.25, +1.25, 1500
    maxiter = 20
    horizon = 2.0 ** 40
    log_horizon = np.log(np.log(horizon)) / np.log(2)
    Z, N = mandelbrot_set(xmin, xmax, ymin, ymax, xn, yn, maxiter, horizon)

    # This line will generate warnings for null values but it is faster to
    # process them afterwards using the nan_to_num
    with np.errstate(invalid='ignore'):
        M = np.nan_to_num(N + 1 -
                          np.log(np.log(abs(Z))) / np.log(2) +
                          log_horizon)

    dpi = 72
    width = 10
    height = 10 * yn / xn
    fig = plt.figure(figsize=(width, height), dpi=dpi)
    ax = fig.add_axes([0.0, 0.0, 1.0, 1.0], frameon=False, aspect=1)

    plt.imshow(M, extent=[xmin, xmax, ymin, ymax], interpolation="bicubic")
    ax.set_xticks([])
    ax.set_yticks([])

    plt.show()