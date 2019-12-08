function close_enough = checkValueDistance( value0, value1,  max, distance )
  distance0 = abs( value0 - value1 );
  distance1 = max - abs( value1 - value0 );
  if ( distance0 <= distance ) || ( distance1 <= distance )
    close_enough = 1;
  else
    close_enough = 0;
  end
end