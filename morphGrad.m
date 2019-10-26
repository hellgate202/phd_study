function output_img = morphGrad( img, win )
  output_img = dilation( img, win ) - erosion( img, win );
end