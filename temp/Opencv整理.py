import cv2

'''
整理了Opencv CV3的库函数在Python中的应用
'''

# 1. 图像导入 视频导入

'''
'''
# 1. 获取图像源

img = cv2.imread("filename",flags=0)

'''

'''
cap = cv2.VideoCapture("VideoCapture")

# 获取当前视频帧
ret, frame = cap.read()

# 2. 图像的基本操作
# 图像变换
"""
改变图像大小

resize(src, dsize[, dst[, fx[, fy[, interpolation]]]]) -> dst

图像缩小一倍的方法
# resize(src, dst, Size(), 0.5, 0.5, interpolation)

1.绝对尺寸法
dsize参数设置为dst大小
2.相对尺寸法
dsize设置为size(0,0), fx,fy设置为需要的比例因子

interpolation:
cv2.INTER_NEAREST : 最近领插值
cv2.INTER_LINEAR  : 双线性插值
cv2.INTER_AREA    : 像素区域重采样
cv2.INTER_CUBIC   : 双三次插值
cv2.INTER_LANCZOS4: 插值（超过 8 x 8个领域

"""
resize = cv2.resize(img,(10,10,10,10))

"""
图像金字塔法

pyrDown(src[, dst[, dstsize[, borderType]]]) -> dst

down = cv2.pyrDown(img，)
"""
down = cv2.pyrDown(img)
up = cv2.pyrUp(img)
# 图像的拉普拉斯金字塔？一般看起来就像是边界
lap_edge = up - down

# 2.图像的处理
# 滤波器就是核大小
'''
自定义边框

copyMakeBorder(src, top, bottom, left, right, borderType[, dst[, value]]) -> dst

cv2.BORDER_CONSTANT : 复制指定的常亮扩展边界
cv2.BORDER_WRAP     : 复制对边的像素扩展边界
cv2.BORDER_REPLICATE: 复制边缘的像素扩展边界
cv2.BORDER_REFLECT  : 通过镜像复制扩展边界
cv2.BORDER_REFLECT_101 ：通过镜像复制扩展边界，边界像素除外
cv2.BORDER_DEFAULT : BORDER_REFLECT_101 的别名
'''
border = cv2.copyMakeBorder(img,10,10,10,10,cv2.BORDER_DEFAULT)

"""
自定义外推

borderInterpolate(p, len, borderType) -> retval
"""

border = cv2.borderInterpolate(100,img.rows,cv2.BORDER_REFLECT_101)
border = cv2.borderInterpolate(-5,img.cols,cv2.BORDER_WRAP)

'''
阈值化操作

threshold(src, thresh, maxval, type[, dst]) -> retval, dst


'''
ret, threshold = cv2.threshold(img,20,255,cv2.THRESH_BINARY)

# 使用 Otsu算法 自动决定最优的阈值
ret, otsu = cv2.threshold(img,20,255,cv2.THRESH_OTSU+cv2.THRESH_BINARY)

"""

adaptiveThreshold(src, maxValue, adaptiveMethod, thresholdType, blockSize, C[, dst]) -> dst

adaptiveMethod 包括两个选项，cv2.ADAPTIVE_THRESH_GAUSSIAN_C 和 cv2.ADAPTIVE_THRESH_MEAN_C
计算方法是 通过计算 bXb区域的加权平均值后减去常数C

当图像中出现比较大的明暗差异时，使用自适应阈值法效果比较好
"""
adaptThreshold = cv2.adaptiveThreshold(img,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,(5,5),10)

'''
简单模糊 与 方框形模糊

blur(src, ksize[, dst[, anchor[, borderType]]]) -> dst
@anchor 指计算时 核与源图像的对齐方式 默认情况是 anchor为cv::Point(-1,-1) 表示核居中

boxFilter(src, ddepth, ksize[, dst[, anchor[, normalize[, borderType]]]]) -> dst

boxFilter 是一种一般化形式，blur 是一种特殊化的形式，两者区别是boxFilter可以以非归一化形式调用，
并且输出的图像深度可以控制（blur输出的图像深度与源图像相同）。
假如变量ddepth值为 -1，输出图像保持和源图像一样的深度，也可以设置为其他形式，比如CV_32F ，CV_8U
'''
blurrd = cv2.blur(img,(3,3))
blurrd = cv2.boxFilter(img,cv2.CV_8U,(3,3))

'''
中值滤波

medianBlur(src, ksize[, dst]) -> dst

'''
blurrd = cv2.medianBlur(img,(5,5))

'''
高斯平滑

GaussianBlur(src, ksize, sigmaX[, dst[, sigmaY[, borderType]]]) -> dst

sigmaX 表示高斯核在x方向的sigma值（最大值的半宽）sigmaY表示y方向
如果只给定了x值，同时y设为0（默认0），那么x和y将相同
如果两者都为0，高斯参数将由系统计算给出
'''
blurrd = cv2.GaussianBlur(img, (5, 5), 0)

'''
双边滤波器

bilateralFilter(src, d, sigmaColor, sigmaSpace[, dst[, borderType]]) -> dst

d 滤波器大小 通常处理视频图像不大于 5

sigmaColor 该值与高斯滤波中的segma参数相似

sigmaSpace 设置为 -1 系统将自动计算。小的sigmaSpace比如10可以带来轻微但明显的效果，
而大的sigmaSpace值比如150可以产生显著的影响，使图像有一种卡通效果
'''
blurrd = cv2.bilateralFilter(img,5,10,10)


# 3.形态学处理
'''
膨胀和腐蚀

腐蚀
erode(src, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst

膨胀
dilate(src, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst

形态学操作
morphologyEx(src, op, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst

开操作通常用来对图像区域进行计数
闭操作用于复杂图像连通分支算法中减少无用或者噪声驱动的片段
开闭操作的迭代不是简单的腐蚀膨胀 腐蚀膨胀，而是 腐蚀腐蚀 膨胀膨胀

形态学梯度
gradient = dilate - erode
梯度操作的结果就是 膨胀减去腐蚀的结果，产生了源图像目标的边缘

顶帽
tophat = src - open
用于显示与其领域相比更亮的部分

blackhat = close - src
用于显示与其领域相比更暗的部分

'''
erode = cv2.erode(img,(5,5),iterations=2)
dilate = cv2.dilate(img,(5,5),iterations=1)
###
open = cv2.morphologyEx(img,cv2.MORPH_OPEN,(5,5),iterations=1)
close = cv2.morphologyEx(img,cv2.MORPH_CLOSE,(5,5))
gradient = cv2.morphologyEx(img,cv2.MORPH_GRADIENT,(5,5))
tophat = cv2.morphologyEx(img,cv2.MORPH_TOPHAT,(5,5))
blackhat = cv2.morphologyEx(img,cv2.MORPH_BLACKHAT,(5,5))

'''
自定义核

getStructuringElement(shape, ksize[, anchor]) -> retval

cv2.MORPH_ELLIPSE : 椭圆
cv2.MORPH_RECT    : 矩形
cv2.MORPH_CROSS   : 交叉
'''
kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(5,5))


# 3. 去噪声
"""
FNLMD 基础算法
输入图像必须是 8bit U8类型
fastNlMeansDenoising(src[, dst[, h[, templateWindowSize[, searchWindowSize]]]]) -> dst
h=3, tempWindow=7, searchWindow=21

推荐参数
噪声 ∂        块大小 s  搜索窗口     衰减参数 h
0< ∂ =<15   |3 x 3  |21 x 21    |0.40 * ∂   |
15< ∂ =<30  |5 x 5  |21 x 21    |0.40 * ∂   |
30< ∂ =<45  |7 x 7  |35 x 35    |0.35 * ∂   |
45< ∂ =<75  |9 x 9  |35 x 35    |0.35 * ∂   |
75< ∂ =<100 |11 x 11|35 x 35    |0.35 * ∂   |

"""
denoising = cv2.fastNlMeansDenoising(img)

"""
彩色图像算法

fastNlMeansDenoisingColored(src[, dst[, h[, hColor[, templateWindowSize[, searchWindowSize]]]]]) -> dst
h=3, hColor=3, tempWindow=7, searchWindow=21

推荐参数
噪声 ∂        块大小 s  搜索窗口     衰减参数 h
0< ∂ =<25   |3 x 3  |21 x 21    |0.55 * ∂   |
25< ∂ =<55  |5 x 5  |35 x 35    |0.40 * ∂   |
55< ∂ =<100 |7 x 7  |35 x 35    |0.35 * ∂   |

"""
color = cv2.fastNlMeansDenoisingColored(img)

"""
适用于顺序帧序列的去噪声方法

1.
fastNlMeansDenoisingMulti(srcImgs, imgToDenoiseIndex, temporalWindowSize[, dst[, h[, templateWindowSize[, searchWindowSize]]]]) -> dst

2.
fastNlMeansDenoisingColoredMulti(srcImgs, imgToDenoiseIndex, temporalWindowSize[, dst[, h[, hColor[, templateWindowSize[, searchWindowSize]]]]]) -> dst

"""
denoising = cv2.fastNlMeansDenoisingMulti()
denoising = cv2.fastNlMeansDenoisingColoredMulti()

"""
直方图均衡化处理

需要注意的是源图必须是 8bit一维图像，所以彩色图像必须分通道处理
"""
Hist = cv2.equalizeHist(img)