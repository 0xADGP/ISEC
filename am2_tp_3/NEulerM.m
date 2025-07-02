%% Método de Euler Melhorado
function [t,y] = NEulerM(f,a,b,n,y0)
%NEULERM Método de Euler Melhorado para resolução de EDO/PVI
%   Variante do Método de Euler que melhora a precisão ao considerar um passo intermediário.
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
    k1 = f(t(i), y(i));
    k2 = f(t(i) + h, y(i) + h * k1);
    y(i+1) = y(i) + (k1 + k2)/2 * h;
end
end