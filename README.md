# contourImageC
Generate the contour of an image in C with a specific color
usage of stb for manipulate image : [https://github.com/nothings/stb](https://github.com/nothings/stb)
# compile line
```
gcc -o contourImage main.c -lm
```

# execution exemple
```
contourImage input.png 15 255 0 0 255 output.png
```
- input.png : image entry
- 15 : 15 pixel around image
- 255 0 0 255 : color around the image
- output.png : output image
