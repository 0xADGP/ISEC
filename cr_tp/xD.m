function xD()
    clc
    clear all
    close all

    % d) [15%] Desenhe manualmente ou numa ferramenta de desenho 5 imagens de cada categoria, que
    %apresentem semelhanças com os exemplos usados no treino da rede.
    %Transcreva os desenhos para matrizes binárias. Desenvolva um pequeno programa para ler um ficheiro
    %correspondente a uma destas imagens e aplicá-lo às melhores redes obtidas em c iv).
    %Como foram classificadas as suas imagens? Registe os resultados e conclusões.
    shapes = ["circle", "kite", "parallelogram", "square", "trapezoid", "triangle"];

    if exist('custom', 'dir')
        disp('Pasta custom encontrada');
    else
        disp('Pasta custom NÃO encontrada');
        return;
    end

    % Estruturas de dados
    imageSize = [28, 28];
    binaryImages = [];
    labels = [];

    for i = 1:length(shapes)
        targetShape = shapes(i);
        disp(['Forma selecionada: ', targetShape]);
        
        pattern = fullfile('custom', targetShape, [char(targetShape), '-custom-*.png']);
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

    binaryImages = binaryImages / max(binaryImages(:));

    disp(['Total de imagens processadas: ', num2str(size(binaryImages, 2))])
    disp(['Tamanho de cada matriz binária: ', mat2str(imageSize)]);

    bestNets = cell(3,1);
    for i = 1:3
        fileName = ['bestNet_c', num2str(i), '.mat'];
        if exist(fileName, 'file')
            load(fileName, 'net');
            bestNets{i} = net;
            
            disp(['Informação da rede carregada: ', fileName]);
            
            topology = [];
            for l = 1:length(net.layers)-1
                topology = [topology, net.layers{l}.size];
            end
            disp(['Topology: ', mat2str(topology)]);
            
            if ~isempty(net.layers{1}.transferFcn)
                disp(['ActivationFcn: ', net.layers{1}.transferFcn]);
            else
                disp('ActivationFcn: Não disponível');
            end
            
            if ~isempty(net.layers{end}.transferFcn)
                disp(['EndTrainFcn: ', net.layers{end}.transferFcn]);
            else
                disp('EndTrainFcn: Não disponível');
            end
            
            if ~isempty(net.performFcn)
                disp(['PerformFcn: ', net.performFcn]);
            else
                disp('PerformFcn: Não disponível');
            end
            
            if ~isempty(net.trainFcn)
                disp(['TrainFcn: ', net.trainFcn]);
            else
                disp('TrainFcn: Não disponível');
            end
            
            if isfield(net.divideParam, 'trainRatio')
                disp(['TrainRatio: ', num2str(net.divideParam.trainRatio)]);
            else
                disp('TrainRatio: Não disponível');
            end
            if isfield(net.divideParam, 'valRatio')
                disp(['ValRatio: ', num2str(net.divideParam.valRatio)]);
            else
                disp('ValRatio: Não disponível');
            end
            if isfield(net.divideParam, 'testRatio')
                disp(['TestRatio: ', num2str(net.divideParam.testRatio)]);
            else
                disp('TestRatio: Não disponível');
            end
            disp('----------------------------------------');
        else
            disp(['Rede ', fileName, ' não encontrada']);
            return;
        end
    end

    results = table([], [], [], [], 'VariableNames', {'NetID', 'Accuracy', 'ConfusionMatrix', 'ClassAccuracies'});
    shapeMap = containers.Map(1:length(shapes), shapes);

    for i=1:3
        net = bestNets{i};
        outputs = net(binaryImages);
        [~, predicted] = max(outputs);
        [~, expected] = max(dummyvar(labels)');

        accuracy = sum(predicted == expected) / length(expected) * 100;

        confusionMatrix = confusionmat(expected, predicted, 'Order', 1:length(shapes));

        classAccuracies = zeros(length(shapes), 1);
        for c = 1:length(shapes)
            trueClass = find(expected == c);
            correctClass = sum(predicted(trueClass) == c);
            if ~isempty(trueClass)
                classAccuracies(c) = correctClass / length(trueClass) * 100;
            else
                classAccuracies(c) = 0;
            end
        end

        results = [results; {i, accuracy, confusionMatrix, classAccuracies}];

        save(['d', num2str(i), '_confusionMatrixCustom.mat'], 'confusionMatrix');

        disp(['Rede ', num2str(i), ':']);
        disp(['Precisão geral: ', num2str(accuracy), '%']);
        disp('Precisão por classe:');
        for c = 1:length(shapes)
            disp([shapes(c), ': ', num2str(classAccuracies(c)), '%']);
        end
    end

    resultsExcel = results(:, {'NetID', 'Accuracy'});
    classAccMatrix = cell2mat(cellfun(@(x) x(:)', results.ClassAccuracies, 'UniformOutput', false));
    classAccuracyTable = array2table(classAccMatrix, 'VariableNames', shapes);
    resultsExcel = [resultsExcel, classAccuracyTable];

    writetable(resultsExcel, 'dResults.xlsx');
end