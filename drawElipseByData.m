function output_img = drawElipseByData( img, class_num, classes, color, block_size )
  [class_cov, mean_x, mean_y] = getClassStat( classes, class_num, block_size );
  [img_size_y, img_size_x, colors] = size( img );
  clear( 'colors' );
  a = class_cov(1,1);
  b = class_cov(1,2);
  c = class_cov(2,2);
  lambda1 = ( a + c ) / 2 + sqrt( ( (a - c) / 2 ) ^ 2 + b ^ 2 );
  lambda2 = ( a + c ) / 2 - sqrt( ( (a - c) / 2 ) ^ 2 + b ^ 2 );
  if b == 0 && a >= c
    theta = 0;
  elseif b == 0 && a < c
    theta = pi / 2;
  else
    theta = atan( ( lambda1 - a ) / b );
  end
  dots = uint16( pi / 0.001 );
  for i = 1 : 1 : dots
    elipse_x = uint16( sqrt( lambda1 * sqrt( 5.991 ) ) * cos( theta ) * cos( double( i ) ) -...
                       sqrt( lambda2 * sqrt( 5.991 ) ) * sin( theta ) * sin( double( i ) ) + mean_x );
    if elipse_x < 1
      elipse_x = 1;
    elseif elipse_x > img_size_x
      elipse_x = img_size_x;
    end
    elipse_y = uint16( sqrt( lambda1 * sqrt( 5.991 ) ) * sin( theta ) * cos( double( i ) ) +...
                       sqrt( lambda2 * sqrt( 5.991 ) ) * cos( theta ) * sin( double( i ) ) + mean_y );
    if elipse_y < 1
      elipse_y = 1;
    elseif elipse_y > img_size_y
      elipse_y = img_size_y;
    end
    img( elipse_y, elipse_x, : ) = color;
    img( elipse_y + 1, elipse_x, : ) = color;
    img( elipse_y, elipse_x + 1, : ) = color;
    img( elipse_y + 1, elipse_x + 1, : ) = color;
  end
  output_img = img;
end

