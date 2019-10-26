function output_img = multiscaleMorphGrad( img, win_list )
  output_img = zeros( size( img ), 'double' );
  for i = 1 : 1 : length( win_list )
    output_img = output_img + morphGrad( double( img ), win_list(i) );
  end
  output_img = cast( output_img ./ length( win_list ), class( img ) );
end