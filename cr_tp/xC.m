function xC()
    clc
    clear all
    close all

    % c) [30%] Nesta tarefa pretende-se um estudo genérico de redes neuronais treinadas e testadas com todas as imagens (pastas start, train, test)
    %Usando as funções de manipulação de imagem do Matlab converta as imagens fornecidas em
    %matrizes binárias. Se achar necessário faça um tratamento prévio às imagens, como
    %redimensionamento, ou outro que achar relevante.
    shapes = ["circle", "kite", "parallelogram", "square", "trapezoid", "triangle"];
    folders = {'start', 'train', 'test'};

    imageSize = [28, 28];
    allBinaryImages = struct();
    allLabels = struct();

    for f=1:length(folders)
        folderName = folders{f};

        if exist(folderName, 'dir')
            disp(['Pasta ', folderName, ' encontrada']);
        else
            disp(['Pasta ', folderName, ' NÃO encontrada']);
            return;
        end

        binaryImages = [];
        labels = [];

        for i=1:length(shapes)
            targetShape = shapes(i);
            disp(['Forma selecionada: ', targetShape, ' folder ', folderName]);

            % Path generalizado das imagens
            pattern = fullfile(folderName, targetShape, sprintf('%s-%s-*.png', targetShape, folderName));
            disp(['Pattern usado: ', pattern])

            folderImages = dir(pattern);
            if isempty(folderImages)
                disp(['Nenhuma imagem encontrada para ', targetShape, ' em ', folderName]);
                continue;
            else
                disp(['Imagens encontradas para ', targetShape, ' em ', folderName]);
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
                    binaryImg = imbinarize(imgResized, 0.5);
    
                    binaryImages = [binaryImages, binaryImg(:)];
                    labels = [labels; categorical(targetShape)];
                end
            end
        end

        allBinaryImages.(folderName) = binaryImages;
        allLabels.(folderName) = labels;
    end

    allImages = [allBinaryImages.start, allBinaryImages.train, allBinaryImages.test];
    allLabelsCombined = [allLabels.start; allLabels.train; allLabels.test];

    disp(['Total de imagens processadas: ', num2str(size(allImages, 2))])

    %i. Recupere as melhores redes neuronais gravadas em b)
    bestNets = cell(3,1);
    for i = 1:3
        fileName = ['bestNet_b', num2str(i), '.mat'];
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

    %Sem treinar as redes verifique qual a classificação dada por cada uma das redes às imagens da
    %pasta test. Registe os resultados obtidos (precisão de teste, matriz de confusão) e compare-os com
    %os desempenhos obtidos em b). Apresente e justifique as conclusões que observou.
    results_ci = table([], [], [], 'VariableNames', {'NetID', 'TestAccuracy', 'ConfusionMatrix'});
    for i=1:3
        net = bestNets{i};
        outputs = net(allBinaryImages.test);
        [~, predicted] = max(outputs);
        [~, expected] = max(dummyvar(allLabels.test)');
        testAccuracy = sum(predicted == expected) / length(expected) * 100;
        confusionMatrix = confusionmat(expected, predicted);

        results_ci = [results_ci; {i, testAccuracy, confusionMatrix}];
        save(['ci_', num2str(i), '_confusionMatrixTest.mat'], 'confusionMatrix');
        disp(['Rede ', num2str(i), ': Precisão da pasta teste = ', num2str(testAccuracy), '%']);
    end
    
    %ii. Treine e teste as melhores redes de b) apenas com as imagens da pasta test. Registe as precisões
    %globais e de teste obtidas na classificação.
    %Após o treino, use as redes para classificar as imagens de cada uma das pastas. Registe as precisões
    %de teste obtidas na classificação:
    %• das imagens da pasta start.
    %• das imagens da pasta train.
    %• das imagens da pasta test.
    %Analise os resultados obtidos. Guarde também a matriz de confusão e registe as conclusões
    %observadas. Qual(ais) a(s) forma(s) com melhor e pior precisões de acerto?
    results_cii = table([], [], [], [], [], [], [], 'VariableNames', {'NetID', 'GlobalAccuracy', 'TestAccuracy', 'StartSetAccuracy', 'TrainSetAccuracy', 'TestSetAccuracy', 'Epochs'});
    
    numRepetitions = 10;

    for i=1:3
        net = bestNets{i};
        globalAccuracies = zeros(numRepetitions, 1);
        testAccuracies = zeros(numRepetitions, 1);
        epochsArray = zeros(numRepetitions, 1);
        confusionMatrixes = cell(numRepetitions, 1);
        bestNet = [];
        bestTestAccuracy = 0;

        for r=1:numRepetitions
            net.divideFcn = 'dividerand';
            net.divideParam.trainRatio = 0.7;
            net.divideParam.valRatio = 0.15;
            net.divideParam.testRatio = 0.15;
            net.TrainParam.showWindow = true;

            [net, tr] = train(net, allBinaryImages.test, dummyvar(allLabels.test)');
            epochsArray(r) = tr.num_epochs;
            outputs = net(allBinaryImages.test);
            [~, predicted] = max(outputs);
            [~, expected] = max(dummyvar(allLabels.test)');
            globalAccuracies(r) = sum(predicted == expected) / length(expected) * 100;

            testIndices = tr.testInd;
            if ~isempty(testIndices)
                testOutputs = net(allBinaryImages.test(:, testIndices));
                [~, testPredicted] = max(testOutputs);
                [~, testExpected] = max(dummyvar(allLabels.test(testIndices))');
                testAccuracies(r) = sum(testPredicted == testExpected) / length(testExpected) * 100;
            else
                testAccuracies(r) = 0;
            end

            if testAccuracies(r) > bestTestAccuracy
                bestTestAccuracy = testAccuracies(r);
                bestNet = net;
            end
        end

        meanGlobalAccuracy = mean(globalAccuracies);
        meanTestAccuracy = mean(testAccuracies);
        meanEpochs = mean(epochsArray);

        outputs_start = bestNet(allBinaryImages.start);
        [~, predicted_start] = max(outputs_start);
        [~, expected_start] = max(dummyvar(allLabels.start)');
        startSetAccuracy = sum(predicted_start == expected_start) / length(expected_start) * 100;
        
        confusionMatrixStart = confusionmat(expected_start, predicted_start);
        %save(['confusionMatrixStart_cii', num2str(i), '.mat'], 'confusionMatrixStart');

        outputs_train = bestNet(allBinaryImages.train);
        [~, predicted_train] = max(outputs_train);
        [~, expected_train] = max(dummyvar(allLabels.train)');
        trainSetAccuracy = sum(predicted_train == expected_train) / length(expected_train) * 100;

        confusionMatrixTrain = confusionmat(expected_train, predicted_train);
        %save(['confusionMatrixTrain_cii', num2str(i), '.mat'], 'confusionMatrixTrain');

        outputs_test = bestNet(allBinaryImages.test);
        [~, predicted_test] = max(outputs_test);
        [~, expected_test] = max(dummyvar(allLabels.test)');
        testSetAccuracy = sum(predicted_test == expected_test) / length(expected_test) * 100;

        confusionMatrixTest = confusionmat(expected_test, predicted_test);
        %save(['confusionMatrixTest_cii', num2str(i), '.mat'], 'confusionMatrixTest');
        
        confusionMatrixes{i} = confusionMatrixStart + confusionMatrixTrain + confusionMatrixTest;
        confusionMatrix = confusionMatrixes{i};
        save(['cii_', num2str(i), '_confusionMatrixTotal.mat'], 'confusionMatrix');
        %save(['confusionMatrixTotal_cii', num2str(i), '.mat'], 'confusionMatrix');

        disp(['Rede ', num2str(i), ': Start = ', num2str(startSetAccuracy), '%: Train = ', num2str(trainSetAccuracy), '%: Test = ', num2str(testSetAccuracy)]);
        results_cii = [results_cii; {i, meanGlobalAccuracy, meanTestAccuracy, startSetAccuracy, trainSetAccuracy, testSetAccuracy, meanEpochs}];
    end

    writetable(results_cii(:, 1:7), 'ciiResults.xlsx');

    %iii. Treine e teste as melhores redes de b) rede com todas imagens (as imagens das pastas start, train
    %e test). Registe as precisões globais e de teste obtidas na classificação.
    %Após o treino, use as redes para classificar as imagens de cada uma das pastas. Registe as precisões
    %de teste obtidas na classificação:
    %• das imagens da pasta start.
    %• das imagens da pasta train.
    %• das imagens da pasta test.
    %Analise os resultados obtidos. Guarde também a matriz de confusão e registe as conclusões
    %observadas. Qual(ais) a(s) forma(s) com melhor e pior precisões de acerto?
    results_ciii = table([], [], [], [], [], [], [], 'VariableNames', {'NetID', 'GlobalAccuracy', 'TestAccuracy', 'StartSetAccuracy', 'TrainSetAccuracy', 'TestSetAccuracy', 'Epochs'});
    
    bestNets_civ = struct('net', {}, 'testAccuracy', {});

    for i=1:3
        net = bestNets{i};
        globalAccuracies = zeros(numRepetitions, 1);
        testAccuracies = zeros(numRepetitions, 1);
        epochsArray = zeros(numRepetitions, 1);
        confusionMatrixes = cell(numRepetitions, 1);
        bestNet = [];
        bestTestAccuracy = 0;

        for r=1:numRepetitions
            net.divideFcn = 'dividerand';
            net.divideParam.trainRatio = 0.7;
            net.divideParam.valRatio = 0.15;
            net.divideParam.testRatio = 0.15;
            net.TrainParam.showWindow = true;

            [net, tr] = train(net, allImages, dummyvar(allLabelsCombined)');
            epochsArray(r) = tr.num_epochs;
            outputs = net(allImages);
            [~, predicted] = max(outputs);
            [~, expected] = max(dummyvar(allLabelsCombined)');
            globalAccuracies(r) = sum(predicted == expected) / length(expected) * 100;

            testIndices = tr.testInd;
            if ~isempty(testIndices)
                testOutputs = net(allImages(:, testIndices));
                [~, testPredicted] = max(testOutputs);
                [~, testExpected] = max(dummyvar(allLabelsCombined(testIndices))');
                testAccuracies(r) = sum(testPredicted == testExpected) / length(testExpected) * 100;
            else
                testAccuracies(r) = 0;
            end

            if testAccuracies(r) > bestTestAccuracy
                bestTestAccuracy = testAccuracies(r);
                bestNet = net;
            end
        end

        meanGlobalAccuracy = mean(globalAccuracies);
        meanTestAccuracy = mean(testAccuracies);
        meanEpochs = mean(epochsArray);

        outputs_start = bestNet(allBinaryImages.start);
        [~, predicted_start] = max(outputs_start);
        [~, expected_start] = max(dummyvar(allLabels.start)');
        startSetAccuracy = sum(predicted_start == expected_start) / length(expected_start) * 100;

        outputs_train = bestNet(allBinaryImages.train);
        [~, predicted_train] = max(outputs_train);
        [~, expected_train] = max(dummyvar(allLabels.train)');
        trainSetAccuracy = sum(predicted_train == expected_train) / length(expected_train) * 100;

        outputs_test = bestNet(allBinaryImages.test);
        [~, predicted_test] = max(outputs_test);
        [~, expected_test] = max(dummyvar(allLabels.test)');
        testSetAccuracy = sum(predicted_test == expected_test) / length(expected_test) * 100;

        confusionMatrixStart = confusionmat(expected_start, predicted_start);
        confusionMatrixTrain = confusionmat(expected_train, predicted_train);
        confusionMatrixTest = confusionmat(expected_test, predicted_test);

        confusionMatrixes{i} = confusionMatrixStart + confusionMatrixTrain + confusionMatrixTest;
        confusionMatrix = confusionMatrixes{i};
        save(['ciii_', num2str(i), '_confusionMatrixTotal.mat'], 'confusionMatrix');

        disp(['Rede ', num2str(i), ': Start = ', num2str(startSetAccuracy), '%: Train = ', num2str(trainSetAccuracy), '%: Test = ', num2str(testSetAccuracy)]);
        results_ciii = [results_ciii; {i, meanGlobalAccuracy, meanTestAccuracy, startSetAccuracy, trainSetAccuracy, testSetAccuracy, meanEpochs}];

        newNet = struct();
        newNet.net = bestNet;
        newNet.testAccuracy = meanTestAccuracy;
        bestNets_civ = [bestNets_civ, newNet];
    end

    writetable(results_ciii(:, 1:7), 'ciiiResults.xlsx');

    %iv. Guarde as três melhores redes obtidas nesta experimentação.
    [~, sortedIndices] = sort([bestNets_civ.testAccuracy], 'descend');
    for i = 1:min(3, length(bestNets_civ))
        idx = sortedIndices(i);
        net = bestNets_civ(idx).net;
        save(['bestNet_c', num2str(i), '.mat'], 'net');
        disp(['Rede ', num2str(i), ' guardada: Precisão total imagens = ', num2str(bestNets_civ(idx).testAccuracy), '%']);
    end
end