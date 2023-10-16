f1 = fopen("datosgrideados2r.raw", "r");
f2 = fopen("datosgrideados2i.raw", "r");

s1 = fread(f1, "float");
s2 = fread(f2, "float");

re = reshape(s1, 2048, 2048);
im = reshape(s2, 2048, 2048);

v = complex(re, im);

I =  fftshift(ifft2(v));

figure;imagesc(abs(I));
colormap('hot');
title('Imagen resultante');