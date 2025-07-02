%% Método de Runge-Kutta de 4ª ordem
function [t,y] = NRK4(f,a,b,n,y0)
%NRK4 Método de Runge-Kutta de quarta ordem para resolução de EDO/PVI
%   Um dos métodos mais usados devido à sua boa precisão e eficiência.
%INPUT:
%   f - função da EDO y' = f(t,y)
%   [a,b] - intervalo da variável independente t
%   n - número de subintervalos
%   y0 - condição inicial
%OUTPUT:
%   y - vetor das soluções aproximadas do PVI
%AUTORES:
% Antonio Domingos Goncalves Pedroso - 2021132042
% Antonio Miguel Grangeiro Rocha - 20221145734
% Samuel Frasao Pinto Costa - 2022161160
% 12/03/2025

h = (b-a)/n;
t = a:h:b;
y = zeros(1, n+1);
y(1) = y0;

for i = 1:n
    k1 = h * f(t(i), y(i));
    k2 = h * f(t(i) + h/2, y(i) + k1/2);
    k3 = h * f(t(i) + h/2, y(i) + k2/2);
    k4 = h * f(t(i+1), y(i) + k3);
    y(i+1) = y(i) + (k1 + 2*k2 + 2*k3 + k4) / 6;
end
end