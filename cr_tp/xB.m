function xB()
    clc
    clear all
    close all

    % b) [30%] Nesta tarefa deve usar apenas as imagens da pasta train
    shapes = ["circle", "kite", "parallelogram", "square", "trapezoid", "triangle"];

    if exist('train', 'dir')
        disp('Pasta train encontrada');
    else
        disp('Pasta train NÃO encontrada');
        return;
    end

    % Estruturas de dados
    imageSize = [28, 28];
    binaryImages = [];
    labels = [];

    for i = 1:length(shapes)
        targetShape = shapes(i);
        disp(['Forma selecionada: ', targetShape]);
        
        pattern = fullfile('train', targetShape, [char(targetShape), '-train-*.png']);
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

    % Configurações da rede
    results = table([], [], [], [], [], [], [], [], [], [], [], 'VariableNames', {'Topology', 'ActivationFcn', 'EndTrainFcn', 'PerformFcn', 'TrainFcn', 'TrainRatio', 'ValRatio', 'TestRatio', 'MeanEpochs', 'MeanGlobalAccuracy', 'MeanTestAccuracy'});

    bestNets = struct('net', {}, 'testAccuracy', {}, 'globalAccuracy', {}, 'config', {});
    bestNetsGlobal = struct('net', {}, 'testAccuracy', {}, 'globalAccuracy', {}, 'config', {});

    topologies = {[50, 30]};
    activationFcns = {'logsig'};
    endTrainFcns = {'softmax'};
    performFcns = {'crossentropy'};
    trainFcns = {'trainrp'};
    splitRatios = [0.8, 0.1, 0.1];
    numRepetitions = 10;
    loop = 0;

    for t = 1:length(topologies)
        topology = topologies{t};
        numLayers = length(topology);
        for af = 1:length(activationFcns)
            actFcn = activationFcns{af};
            for ef = 1:length(endTrainFcns)
                endFcn = endTrainFcns{ef};
                for pf = 1:length(performFcns)
                    perfFcn = performFcns{pf};
                    for tf = 1:length(trainFcns)
                        trFcn = trainFcns{tf};
                        for sr = 1:size(splitRatios, 1)
                            trainRatio = splitRatios(sr, 1);
                            valRatio = splitRatios(sr, 2);
                            testRatio = splitRatios(sr, 3);
                            loop = loop + 1;

                            disp(['Testando: Topologia ', mat2str(topology), ', Ativação ', actFcn, ', Saída ', endFcn, ', Perda ', perfFcn, ', Treino ', trFcn, ', Divisão ', num2str(trainRatio*100), '/', num2str(valRatio*100), '/', num2str(testRatio*100)]);

                            globalAccuracies = zeros(numRepetitions, 1);
                            arrayEpochs = zeros(numRepetitions, 1);
                            testAccuracies = zeros(numRepetitions, 1);
                            confusionMatrices = cell(numRepetitions, 1);
                            bestNet = [];
                            bestTestAccuracy = 0;
                            bestGlobalAccuracy = 0;

                            for r = 1:numRepetitions
                                net = feedforwardnet(topology);
                                net.trainFcn = trFcn;
                                for l = 1:numLayers
                                    net.layers{l}.transferFcn = actFcn;
                                end
                                net.layers{end}.transferFcn = endFcn;
                                net.performFcn = perfFcn;
                                net.divideFcn = 'dividerand';
                                net.divideParam.trainRatio = trainRatio;
                                net.divideParam.valRatio = valRatio;
                                net.divideParam.testRatio = testRatio;

                                net.trainParam.showWindow = true;
                                net.trainParam.goal = 1e-7;
                                net.trainParam.epochs = 5000;

                                [net, tr] = train(net, binaryImages, dummyvar(labels)');
                                arrayEpochs(r) = tr.num_epochs;

                                outputs = net(binaryImages);
                                [~, predicted] = max(outputs);
                                [~, expected] = max(dummyvar(labels)');
                                globalAccuracies(r) = sum(predicted == expected) / length(expected) * 100;

                                testIndices = tr.testInd;
                                if ~isempty(testIndices)
                                    testOutputs = net(binaryImages(:, testIndices));
                                    [~, testPredicted] = max(testOutputs);
                                    [~, testExpected] = max(dummyvar(labels(testIndices))');
                                    [confusionMatrices{r}, order] = confusionmat(testExpected, testPredicted, 'Order', 1:length(shapes));
                                    testAccuracies(r) = sum(testPredicted == testExpected) / length(testExpected) * 100;
                                else
                                    testAccuracies(r) = 0;
                                    confusionMatrices{r} = zeros(length(shapes), length(shapes));
                                end

                                if testAccuracies(r) > bestTestAccuracy
                                    bestTestAccuracy = testAccuracies(r);
                                    bestGlobalAccuracy = globalAccuracies(r);
                                    bestNet = net;
                                elseif testAccuracies(r) == bestTestAccuracy && globalAccuracies(r) > bestGlobalAccuracy
                                    bestGlobalAccuracy = globalAccuracies(r);
                                    bestNet = net;
                                end
                            end

                            meanEpochs = mean(arrayEpochs);
                            disp(['Média de epochs: ', num2str(meanEpochs), ' epochs']);

                            meanGlobalAccuracy = mean(globalAccuracies);
                            disp(['Precisão global da rede: ', num2str(meanGlobalAccuracy), '%']);

                            meanTestAccuracy = mean(testAccuracies);
                            disp(['Precisão média de teste: ', num2str(meanTestAccuracy), '%']);

                            results = [results; {mat2str(topology), actFcn, endFcn, perfFcn, trFcn, trainRatio, valRatio, testRatio, meanEpochs, meanGlobalAccuracy, meanTestAccuracy}];

                            meanConfusionMatrix = zeros(length(shapes), length(shapes));
                            validRepetitions = 0;
                            for r = 1:numRepetitions
                                if ~isempty(confusionMatrices{r}) && all(size(confusionMatrices{r}) == [length(shapes), length(shapes)])
                                    meanConfusionMatrix = meanConfusionMatrix + confusionMatrices{r};
                                    validRepetitions = validRepetitions + 1;
                                end
                            end
                            if validRepetitions > 0
                                meanConfusionMatrix = meanConfusionMatrix / validRepetitions;
                            else
                                meanConfusionMatrix = zeros(length(shapes), length(shapes));
                            end
                            save(['confusionMatrix_', num2str(t), '_', num2str(af), '_', num2str(ef), '_', num2str(pf), '_', num2str(tf), '_', num2str(sr), '.mat'], 'meanConfusionMatrix');

                            if bestTestAccuracy > 0 && ~isempty(bestNet)
                                newNet = struct();
                                newNet.net = bestNet;
                                newNet.testAccuracy = bestTestAccuracy;
                                newNet.globalAccuracy = bestGlobalAccuracy;
                                newNet.config = {mat2str(topology), actFcn, endFcn, perfFcn, trFcn, trainRatio, valRatio, testRatio};
                                bestNets = [bestNets, newNet];
                                bestNetsGlobal = [bestNetsGlobal, newNet];
                            end
                        end
                    end
                end
            end
        end
    end

    disp(['Total de configurações testadas: ', num2str(loop)])

    writetable(results, 'bResults.xlsx')

    if isempty(bestNets)
        disp('Nenhuma rede válida encontrada.');
        return;
    end

    if ~isempty(bestNets)
        [~, sortedIndices] = sort([bestNets.testAccuracy], 'descend');
        for i = 1:min(3, length(bestNets))
            idx = sortedIndices(i);
            net = bestNets(idx).net;
            config = bestNets(idx).config;
            save(['bBestNetTest_', num2str(i), '.mat'], 'net');
            disp(['Rede (Teste) ', num2str(i), ' guardada: Topologia ', config{1}, ', Ativação ', config{2}, ', Saída ', config{3}, ', Perda ', config{4}, ', Treino ', config{5}, ', Divisão ', num2str(config{6}*100), '/', num2str(config{7}*100), '/', num2str(config{8}*100), ', Precisão de teste: ', num2str(bestNets(idx).testAccuracy), '%', ', Precisão global: ', num2str(bestNets(idx).globalAccuracy), '%']);
        end
    else
        disp('Nenhuma rede guardada devido a erro anterior (test accuracy).');
    end

    if ~isempty(bestNetsGlobal)
        [~, sortedIndices] = sort([bestNetsGlobal.globalAccuracy], 'descend');
        for i = 1:min(3, length(bestNetsGlobal))
            idx = sortedIndices(i);
            net = bestNetsGlobal(idx).net;
            config = bestNetsGlobal(idx).config;
            save(['bBestNetGlobal_', num2str(i), '.mat'], 'net');
            disp(['Rede (Global) ', num2str(i), ' guardada: Topologia ', config{1}, ', Ativação ', config{2}, ', Saída ', config{3}, ', Perda ', config{4}, ', Treino ', config{5}, ', Divisão ', num2str(config{6}*100), '/', num2str(config{7}*100), '/', num2str(config{8}*100), ', Precisão de teste: ', num2str(bestNetsGlobal(idx).testAccuracy), '%', ', Precisão global: ', num2str(bestNetsGlobal(idx).globalAccuracy), '%']);
        end
    else
        disp('Nenhuma rede guardada devido a erro anterior (global accuracy).');
    end

    [~, bestIdx] = max([bestNets.testAccuracy]);
    bestNet = bestNets(bestIdx).net;
    bestConfig = bestNets(bestIdx).config;
    disp(['Melhor configuração (Teste): Topologia ', bestConfig{1}, ', Ativação ', bestConfig{2}, ', Saída ', bestConfig{3}, ', Perda ', bestConfig{4}, ', Treino ', bestConfig{5}, ', Divisão ', num2str(bestConfig{6}*100), '/', num2str(bestConfig{7}*100), '/', num2str(bestConfig{8}*100), ', Precisão de teste: ', num2str(bestNets(bestIdx).testAccuracy), '%', ', Precisão global: ', num2str(bestNets(bestIdx).globalAccuracy), '%']);

    [~, bestIdx] = max([bestNetsGlobal.globalAccuracy]);
    bestConfig = bestNetsGlobal(bestIdx).config;
    disp(['Melhor configuração (Global): Topologia ', bestConfig{1}, ', Ativação ', bestConfig{2}, ', Saída ', bestConfig{3}, ', Perda ', bestConfig{4}, ', Treino ', bestConfig{5}, ', Divisão ', num2str(bestConfig{6}*100), '/', num2str(bestConfig{7}*100), '/', num2str(bestConfig{8}*100), ', Precisão de teste: ', num2str(bestNetsGlobal(bestIdx).testAccuracy), '%', ', Precisão global: ', num2str(bestNetsGlobal(bestIdx).globalAccuracy), '%']);

    outputs = bestNet(binaryImages);
    [~, predicted] = max(outputs);
    [~, expected] = max(dummyvar(labels)');
    confusionMatrix = confusionmat(expected, predicted);

    classAccuracies = zeros(length(shapes), 1);
    for c = 1:length(shapes)
        trueClass = find(expected == c);
        correctClass = sum(predicted(trueClass) == c);
        classAccuracies(c) = correctClass / length(trueClass) * 100;
    end

    disp('Precisão por forma:');
    for c = 1:length(shapes)
        disp([shapes(c), ': ', num2str(classAccuracies(c)), '%']);
    end
    [bestAcc, bestClass] = max(classAccuracies);
    [worstAcc, worstClass] = min(classAccuracies);
    disp(['Forma com melhor precisão: ', shapes(bestClass), ' (', num2str(bestAcc), '%)']);
    disp(['Forma com pior precisão: ', shapes(worstClass), ' (', num2str(worstAcc), '%)']);

    save('bBestNetCM.mat', 'confusionMatrix');
end