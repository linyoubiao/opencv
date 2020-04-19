2020-04-19
���μ�Ӧ��

"""
��Ȥ������
û�н���
2020/4/17 16:55
"""
import numpy as np
 
 
def mandelbrot_set(xmin, xmax, ymin, ymax, xn, yn, maxiter, horizon=2.0):
    X = np.linspace(xmin, xmax, xn)
    Y = np.linspace(ymin, ymax, yn)
    C = X + Y[:, None] * 1j
    N = np.zeros_like(C, dtype=int)
    Z = np.zeros_like(C)
    for n in range(maxiter):
        I = np.less(abs(Z), horizon)
        N[I] = n
        Z[I] = Z[I]**2 + C[I]
    N[N == maxiter-1] = 0
    return Z, N
 
 
if __name__ == '__main__':
    import time
    import matplotlib
    from matplotlib import colors
    import matplotlib.pyplot as plt
 
    xmin, xmax, xn = -2.25, +0.75, 1500
    ymin, ymax, yn = -1.25, +1.25, 1500
    maxiter = 20
    horizon = 2.0 ** 40
    log_horizon = np.log(np.log(horizon))/np.log(2)
    Z, N = mandelbrot_set(xmin, xmax, ymin, ymax, xn, yn, maxiter, horizon)
 
    # Normalized recount as explained in:
    # https://linas.org/art-gallery/escape/smooth.html
    # https://www.ibm.com/developerworks/community/blogs/jfp/entry/My_Christmas_Gift
 
    # This line will generate warnings for null values but it is faster to
    # process them afterwards using the nan_to_num
    with np.errstate(invalid='ignore'):
        M = np.nan_to_num(N + 1 -
                          np.log(np.log(abs(Z)))/np.log(2) +
                          log_horizon)
 
    dpi = 72
    width = 10
    height = 10*yn/xn
    fig = plt.figure(figsize=(width, height), dpi=dpi)
    ax = fig.add_axes([0.0, 0.0, 1.0, 1.0], frameon=False, aspect=1)
 
    # Shaded rendering
    # light = colors.LightSource(azdeg=315, altdeg=10)
    # M = light.shade(M, cmap=plt.cm.hot, vert_exag=1.5,
    #                 norm=colors.PowerNorm(0.3), blend_mode='hsv')
    plt.imshow(M, extent=[xmin, xmax, ymin, ymax], interpolation="bicubic")
    ax.set_xticks([])
    ax.set_yticks([])
 
    # Some advertisement for matplotlib
    # year = time.strftime("%Y")
    # text = ("The Mandelbrot fractal set\n"
    #         "Rendered with matplotlib %s, %s - http://matplotlib.org"
    #         % (matplotlib.__version__, year))
    # ax.text(xmin+.025, ymin+.025, text, color="white", fontsize=12, alpha=0.5)
 
    plt.show()
��������������������������������
��Ȩ����������ΪCSDN������islinyoubiao����ԭ�����£���ѭ CC 4.0 BY-SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
ԭ�����ӣ�https://blog.csdn.net/islinyoubiao/article/details/105582593