%% Método ODE45
function [t,y] = mODE45(f,a,b,n,y0)
%ODE45 Utilização da função do MATLAB ODE45 para resolver EDO/PVI
%   Utiliza um método adaptativo de Runge-Kutta de quarta e quinta ordem.
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

[~, y] = ode45(f, t, y0);
y = y';
end