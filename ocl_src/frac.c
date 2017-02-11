typedef double2 Complex;

void kernel frac(global const float* A,
                       global const int* B,
                       global int* C)
{

  int size = B[0];
  double x = get_global_id(0) / (size);
  double y = get_global_id(0) % (size);

  Complex zn;
  Complex c;

  c.s0= A[0];
  c.s1= A[1];

  zn.s0 = (((x/size) * 3 ) - 1.5 );
  zn.s1 = (((y/size) * 3 ) - 1.5);

  int i = 0;
    do
      {
      double tmp = zn.s0;
      zn.s0 = zn.s0 * zn.s0 - zn.s1 * zn.s1+c.s0 ;
      zn.s1= 2.0 * tmp * zn.s1 + c.s1;
      i++;
      }
    while( (zn.s0 * zn.s0) + (zn.s1 * zn.s1) < 4.0 &&
           i<126
           );

    float val = ((float)(126-i))/126;

    int R = 256 * val* val;
    int G = 256 * val * val* val ;
    int Bl =256 * val ;
    C[get_global_id(0)] = (Bl<<16) +(G << 8) + R ;
}
