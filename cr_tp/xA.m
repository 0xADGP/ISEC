function xA()
    clc
    clear all
    close all

    % a) [15%] Nesta tarefa deve usar apenas as imagens da pasta start
    %Usando as funções de manipulação de imagem do Matlab converta as imagens fornecidas em
    %matrizes binárias. Se achar necessário faça um tratamento prévio às imagens, como
    %redimensionamento, ou outro que achar relevante.
    shapes = ["circle", "kite", "parallelogram", "square", "trapezoid", "triangle"];

    if exist('start', 'dir')
        disp('Pasta start encontrada');
    else
        disp('Pasta start NÃO encontrada');
        return;
    end 

    % Estruturas de dados
    imageSize = [28, 28];
    binaryImages = [];
    labels = [];

    for i = 1:length(shapes)
        targetShape = shapes(i);
        disp(['Forma selecionada: ', targetShape]);
        
        % Path generalizado das imagens
        pattern = fullfile('start', targetShape, [char(targetShape), '-start-*.png']);
        disp(['Pattern usado: ', pattern])

        folderImages = dir(pattern);
        if isempty(folderImages)
            disp(['Nenhuma imagem encontrada para ', targetShape]);
            continue;
        else
            disp(['Imagens encontradas para ', targetShape]);
            for j=1:length(folderImages)
                disp(folderImages(j).name);

                fullPath = fullfile(folderImages(j).folder, folderImages(j).name);
                img = imread(fullPath);

                %IMPORTANTE PARA REDUZIR SIGNIFICAMENTE A MATRIZ DAS IMAGENS
                % 0.298936021293775 * R + 0.587043074451121 * G + 0.114020904255103 * B 
                if size(img, 3) == 3
                    img = rgb2gray(img);
                end

                imgResized = imresize(img, imageSize);
                binaryImg = imbinarize(imgResized);

                binaryImages = [binaryImages, binaryImg(:)];
                labels = [labels; categorical(targetShape)];
            end
        end
    end

    disp(['Total de imagens processadas: ', num2str(size(binaryImages, 2))])
    disp(['Tamanho de cada matriz binária: ', mat2str(imageSize)]);

    %Comece por uma rede neuronal feedforward de uma camada com dez neurónios. Use a rede para
    %treinar as figuras geométricas que se encontram na pasta start. Nesta pasta encontram-se 5 imagens
    %de cada forma.
    %Use todos os exemplos no treino. Deverá encontrar uma (ou várias) parametrização(ões) da rede
    %capazes de reconhecer todas as imagens corretamente (precisão global de 100%).

    results = table([], [], [], [], 'VariableNames', {'NumLayers', 'NumNeurons', 'MeanAccuracy', 'Epochs'});
    numIt = 10;
    accuracies = zeros(numIt, 1);
    bestAccuracy = 0;

    %https://www.mathworks.com/matlabcentral/answers/259381-neural-net-toolbox-dividefcn
    for i = 1:numIt
        net = feedforwardnet(10);

        %net.divideFcn = '';
        net.divideFcn = 'dividerand';
        net.divideParam.trainRatio = 1;
        net.divideParam.valRatio = 0;
        net.divideParam.testRatio = 0;

        net.trainParam.showWindow = true;

        net.trainParam.goal = 1e-2;
        
        [net, tr] = train(net, binaryImages, dummyvar(labels)');

        %output da rede
        outputs = net(binaryImages);
        [~, predicted] = max(outputs);
        [~, expected] = max(dummyvar(labels)');
        disp(tr.num_epochs)
        accuracies(i) = sum(predicted==expected) / length(expected) * 100

        if accuracies(i) > bestAccuracy
            bestAccuracy = accuracies(i);
        end
    end

    meanAccuracy = mean(accuracies);
    disp(['Precisão média (1 camada, 10 neurônios, ', num2str(numIt), ' repetições): ', num2str(meanAccuracy), '%']);
    results = [results; {1, '10', meanAccuracy, tr.num_epochs}];

    %Mantenha as funções de ativação e de treino default e teste outras topologias para a rede neuronal
    %(mais neurónios/camadas).
    %Em cada parametrização de ii) e iii) registe a precisão global da rede e compare os resultados
    %obtidos.
    topologies = {[20], [50], [10, 10], [20, 10], [50, 20]};

    for t=1:length(topologies)
        numNeuronios = topologies{t};
        numLayers = length(numNeuronios);
        disp(['Treinando rede com ', num2str(numLayers), ' camada(s): ', mat2str(numNeuronios)]);

        accuracies = zeros(numIt, 1);
        bestAccuracy = 0;

        for i = 1:numIt
            net = feedforwardnet(numNeuronios);
            %net.divideFcn = '';
            net.divideFcn = 'dividerand';
            net.divideParam.trainRatio = 1;
            net.divideParam.valRatio = 0;
            net.divideParam.testRatio = 0;
            net.trainParam.showWindow = true;

            net.trainParam.goal = 1e-6;
        
            [net, tr] = train(net, binaryImages, dummyvar(labels)');

            %output da rede
            outputs = net(binaryImages);
            [~, predicted] = max(outputs);
            [~, expected] = max(dummyvar(labels)');
            disp(tr.num_epochs)
            accuracies(i) = sum(predicted==expected) / length(expected) * 100
    
            if accuracies(i) > bestAccuracy
                bestAccuracy = accuracies(i);
            end
        end

        meanAccuracy = mean(accuracies);
        disp(['Precisão média (', num2str(numLayers), ' camada(s), ', mat2str(numNeuronios), ', ', num2str(numIt), ' repetições): ', num2str(meanAccuracy), '%']);
        results = [results; {numLayers, mat2str(numNeuronios), meanAccuracy, tr.num_epochs}];
    end

    writetable(results, 'aResults.xlsx');
    disp('Resultados finais:');
    disp(results);
end