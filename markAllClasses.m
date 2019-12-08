function output_img = markAllClasses( img, classes_amount, classes, figure, block_size )
  output_img = img;
  color_interval = 2 ^ 24 / classes_amount;
  for i = 1 : 1 : classes_amount
    color = color_interval * i;
    red = uint8( mod( color, 2 ^ 8 ) );
    green = uint8( mod( color, 2 ^ 16 ) / 2 ^ 8 );
    blue = uint8( color / 2 ^ 16 );
    if figure == "rectangle"
      output_img = placeBoxesOnClass( output_img, i, classes, [red,green,blue] );
    elseif figure == "elipse"
      output_img = drawElipseByData( output_img, i, classes, [red,green,blue], block_size );
    end
  end
end