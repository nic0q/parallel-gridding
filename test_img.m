f1 = fopen("datosgrideadosr.raw", "r");
f2 = fopen("datosgrideadosi.raw", "r");

s1 = fread(f1, "float");
s2 = fread(f2, "float");

re = reshape(s1, 2048, 2048);
im = reshape(s2, 2048, 2048);

v = complex(re, im);

I =  fftshift(ifft2(v));

imagesc(abs(I));
colormap('gray');
title('Imagen resultante');