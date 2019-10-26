function output_img = erosion( img, win )
  [img_size_y, img_size_x] = size( img );
  img = imresize( img, size( img ) + ( win - 1 ) );
  output_img = zeros( img_size_y, img_size_x, class( img ) );
  for y = 1 : 1 : img_size_y
    for x = 1 : 1 : img_size_x
      patch = img(y : y + win - 1,x : x + win - 1);
      output_img(y,x) = min( patch, [], 'all' );
    end
  end
end