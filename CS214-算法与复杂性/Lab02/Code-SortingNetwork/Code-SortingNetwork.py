import sys

if sys.version_info.major == 2:
    from Tkinter import *
elif sys.version_info.major == 3:
    from tkinter import *


class MyCanvas(Canvas):
    def __init__(self, master, hLineWidth=1, vLineWidth=1, radius=2, **kwargs):
        Canvas.__init__(self, master, kwargs)
        self.hLineWidth = hLineWidth
        self.vLineWidth = vLineWidth
        self.radius = radius

    def create_segment_h(self, x, y, l):
        self.create_line(x, y, x + l, y, width=self.hLineWidth)
        self.create_oval(x - self.radius, y - self.radius, x + self.radius, y + self.radius, fill='black')
        self.create_oval(x + l - self.radius, y - self.radius, x + l - self.radius, y + self.radius, fill='black')

    def create_segment_v(self, x, y, l):
        self.create_line(x, y, x, y + l, width=self.vLineWidth)
        self.create_oval(x - self.radius, y - self.radius, x + self.radius, y + self.radius, fill='black')
        self.create_oval(x - self.radius, y + l - self.radius, x + self.radius, y + l + self.radius, fill='black')

    def create_line_h(self, x, y, l):
        self.create_line(x, y, x + l, y, width=self.hLineWidth)

    def create_line_v(self, x, y, l):
        self.create_line(x, y, x, y + l, width=self.vLineWidth)


class HalfCleaner:
    def __init__(self, size):
        self.size = size

    def hNum(self):
        return self.size // 2 if self.size > 1 else 0

    def draw(self, cvs, x, y, hScale, vScale):
        if self.size > 1:
            for i in range(self.size):
                cvs.create_line_h(x, y + i * vScale, self.hNum() * hScale)
            for i in range(self.size // 2):
                cvs.create_segment_v(x + (i + 0.5) * hScale, y + i * vScale, (self.size // 2) * vScale)


class BitonicSorter:
    def __init__(self, size):
        self.size = size
        if self.size > 1:
            self.halfCleaner = HalfCleaner(self.size)
            self.subSorter = BitonicSorter(self.size // 2)

    def hNum(self):
        return self.halfCleaner.hNum() + self.subSorter.hNum() if self.size > 1 else 0

    def draw(self, cvs, x, y, hScale, vScale):
        if self.size > 1:
            self.halfCleaner.draw(cvs, x, y, hScale, vScale)
            self.subSorter.draw(cvs, x + self.halfCleaner.hNum() * hScale, y,
                                hScale, vScale)
            self.subSorter.draw(cvs, x + self.halfCleaner.hNum() * hScale, y + (self.size // 2) * vScale,
                                hScale, vScale)


class Merger:
    def __init__(self, size):
        self.size = size
        if size > 1:
            self.subSorter = BitonicSorter(size // 2)

    def hNum(self):
        return self.size // 2 + self.subSorter.hNum() if self.size > 1 else 0

    def draw(self, cvs, x, y, hScale, vScale):
        if self.size > 1:
            for i in range(self.size):
                cvs.create_line_h(x, y + i * vScale, (self.size // 2) * hScale)
            for i in range(self.size // 2):
                cvs.create_segment_v(x + (i + 0.5) * hScale, y + i * vScale, (self.size - 1 - 2 * i) * vScale)
            self.subSorter.draw(cvs, x + (self.size // 2) * hScale, y, hScale, vScale)
            self.subSorter.draw(cvs, x + (self.size // 2) * hScale, y + (self.size // 2) * vScale, hScale, vScale)


class Sorter:
    def __init__(self, size):
        self.size = size
        if size > 1:
            self.subSorter = Sorter(size // 2)
            self.merger = Merger(size)

    def hNum(self):
        return self.subSorter.hNum() + self.merger.hNum() if self.size > 1 else 0

    def draw(self, cvs, x, y, hScale, vScale):
        if self.size > 1:
            self.subSorter.draw(cvs, x, y, hScale, vScale)
            self.subSorter.draw(cvs, x, y + (self.size // 2) * vScale, hScale, vScale)
            self.merger.draw(cvs, x + self.subSorter.hNum() * hScale, y, hScale, vScale)


if __name__ == '__main__':
    k = int(input('please input the number k: '))
    n = 2 ** k
    sortingNetwork = Sorter(n)

    winW, winH = 2400 * 0.6, 1500 * 0.6
    hMargin, vMargin = winW // 20, winH // 20
    hScale, vScale = (winW - 2 * hMargin) // sortingNetwork.hNum(), (winH - 2 * vMargin) // (n - 1)

    root = Tk()
    root.title('Sorting Network with n=%d (Drawn by Python Tkinter)' % n)
    cvs = MyCanvas(root, bg='white', width=winW, height=winH)
    sortingNetwork.draw(cvs, hMargin, vMargin, hScale, vScale)
    cvs.pack()
    root.mainloop()
