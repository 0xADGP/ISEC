%% Método de Runge-Kutta de 2ª ordem
function [t,y] = NRK2(f,a,b,n,y0)
%NRK2 Método de Runge-Kutta de segunda ordem para resolução de EDO/PVI
%   Variante mais precisa que o Método de Euler.
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

h = (b-a)/n; % amplitude do intervalo
t = a:h:b; % vetor t
y = zeros(1,n+1); % alocação de memória para o vetor y
y(1) = y0;

for i =1:n
    k1 = h*f(t(i),y(i));
    k2 = h*f(t(i+1),y(i)+k1);
    y(i+1) = y(i)+(k1+k2)/2;
end
end