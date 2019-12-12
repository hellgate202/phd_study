function gfd = getGFD( img, rad_amount, ang_amount, y_center, x_center, lambda1, lambda2, theta )
img = rgb2gray( img );
gfd = zeros( rad_amount, ang_amount, 'double' );
rad_values = zeros( rad_amount, ang_amount, 'double' );
for ang = 1 : 1 : rad_amount
  cur_ang = ( ( 2 * pi ) / ang_amount ) * ( ang - 1 );
  elipse_y = uint16( sqrt( lambda1 * sqrt( 5.991 ) ) * sin( theta ) * cos( double( cur_ang ) ) +...
                     sqrt( lambda2 * sqrt( 5.991 ) ) * cos( theta ) * sin( double( cur_ang ) ) + y_center );
  elipse_x = uint16( sqrt( lambda1 * sqrt( 5.991 ) ) * cos( theta ) * cos( double( cur_ang ) ) -...
                     sqrt( lambda2 * sqrt( 5.991 ) ) * sin( theta ) * sin( double( cur_ang ) ) + x_center );
  dxr = ( elipse_x - x_center ) / rad_amount;
  dyr = ( elipse_y - y_center ) / rad_amount;
  for rad = 1 : 1 : rad_amount
    radius_coords_x = ceil( x_center + dxr * rad );
    radius_coords_y = ceil( y_center + dyr * rad );
    rad_values(rad,ang) = img(radius_coords_y,radius_coords_x);
  end
end
rad_values = complex( rad_values );
for R = 1 : 1 : rad_amount
  for T = 1 : 1 : ang_amount
    r_vec = zeros( rad_amount, 1, 'double' );
    for r = 1 : 1 : rad_amount
      t_vec = zeros( ang_amount, 1, 'double' );
      for t = 1 : 1 : ang_amount
        cur_ang = ( ( 2 * pi ) / ang_amount ) * ( t - 1 );
        complex_exp = 2 * pi * r * ( cur_ang / R );
        real_exp = 2 * pi * ( cur_ang / T );
        t_vec(t) = complex( rad_values(r,t) ) * complex( exp( -1i * complex( complex_exp ) + complex( real_exp ) ) );
      end
     r_vec(r) = sum( t_vec );
    end
    gfd(R,T) = sum( r_vec );
  end
end
end