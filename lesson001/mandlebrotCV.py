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
    # np.zeros_lick(a) 顾名思义这个函数的意思就是生成一个和你所给数组a相同shape的全0数组。
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
    # nan_to_num: 使用0代替数组x中的nan元素，使用有限的数字代替inf元素
    with np.errstate(invalid='ignore'):
        M = np.nan_to_num(N + 1 -
                          np.log(np.log(abs(Z))) / np.log(2) +
                          log_horizon)

    cv.namedWindow("mandlebrot", cv.WINDOW_NORMAL)
    cv.imshow("mandlebrot", M)
    cv.waitKey(0)
    cv.destroyAllWindows()