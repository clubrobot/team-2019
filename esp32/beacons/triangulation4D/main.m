close all;
clear all;
Ztag = 484.3;   % offset en Z du tag
posAnchors = [5 -49 438.3 ; 1000 3049 438.3 ; 1950 -49 438.3 ; 21.18 1326 1016.3];           % position des balises fixes en mm (x, y, z)
x1 = posAnchors(1,1);
x2 = posAnchors(2,1);
x3 = posAnchors(3,1);
x4 = posAnchors(4,1);
y1 = posAnchors(1,2);
y2 = posAnchors(2,2);
y3 = posAnchors(3,2);
y4 = posAnchors(4,2);
z1 = posAnchors(1,3);
z2 = posAnchors(2,3);
z3 = posAnchors(3,3);
z4 = posAnchors(4,3);

mes = [1100 ; 2297 ; 1580 ; 900] ;                    % mesures entre tag et anchors en mm
d1 = sqrt(mes(1)^2 - abs(z1 - Ztag)^2);                     % projection dans le plan des tags
d2 = sqrt(mes(2)^2 - abs(z2 - Ztag)^2);                     % projection dans le plan des tags
d3 = sqrt(mes(3)^2 - abs(z3 - Ztag)^2);                     % projection dans le plan des tags
d4 = sqrt(mes(4)^2 - abs(z4 - Ztag)^2);                     % projection dans le plan des tags

A = [x1-x4 y1-y4 ; x2-x4 y2-y4 ; x3-x4 y3-y4 ];
A = -2*A;

b = [ d1^2-x1^2-y1^2-d4^2+x4^2+y4^2 ; d2^2-x2^2-y2^2-d4^2+x4^2+y4^2 ; d3^2-x3^2-y3^2-d4^2+x4^2+y4^2];
p = inv(A'*A)*A'*b

figure 

background = imread('terrain.PNG');                 % chargement de l'image de fond
imagesc([0 2000], [0 3000], flip(background, 1));   % mise à l'échelle de l'image 
set(gca,'ydir','normal');                           % retourne l'axe y

viscircles(posAnchors(:,1:2),mes);             % trace les cercles
axis equal;                             % repère orthonormé
hold on
plot(p(1),p(2),'r*')

