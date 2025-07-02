%% Método de Runge-Kutta de 3ª ordem
function [t,y] = NRK3(f,a,b,n,y0)
%NRK3 Método de Runge-Kutta de terceira ordem para resolução de EDO/PVI
%   Oferece maior precisão que as variantes de ordem inferior.
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
t(1) = a;
y(1) = y0;

for i = 1:n
    t(i+1) = t(i) + h;
    k1 = h * f(t(i), y(i));
    k2 = h * f(t(i) + h/2, y(i) + k1/2);
    k3 = h * f(t(i) + h, y(i) - k1 + 2 * k2);
    y(i+1) = y(i) + (1/6) * (k1 + 4 * k2 + k3);
end
end