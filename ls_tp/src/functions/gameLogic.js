import randomWords from "./words";
import { updateRwords } from "../components/hud-container/words-panel/wordsPanel.component";

let words = [];
let discoveredWords = []; //Guarda as palavras discobertas;
let discoveredPos = []; //Guarda as posicoes das palavras discobertas

/**
 * Reseta todas as vars que possam ter sido alteradas durante o jogo
 * Constroi o tabuleiro e popula-o com letras random
 * Chama a func spanWords para popular a tabela com palavras random
 * @returns table -> devolve arrauy com tabuleiro construido
 */
function buildTable(){
    teste(false); //FUNCAO PARA TESTE =========> APAGAR LINHA E FUNCAO NO FINAL 
    let tWidth = getTableW();
    let table=[];

    words = [];
    discoveredWords = [];
    discoveredPos = [];

    let alphabet = "abcdefghijklmnopqrstuvwxyz"
    for(let rows=0; rows<tWidth; rows++){
        table.push([]);
        for(let cols=0; cols<tWidth; cols++){
            /*1*/ table[rows].push([alphabet[Math.floor(Math.random()*alphabet.length)]]);
            /*2*/ //table[rows].push(" ");
            //Para desativar as letras tira de comentario o 2 e mete o 1 a comentario
        }
    }
    spawnWords(tWidth, table);
    return table;
}

//TESTES DURANTE O DESENVOLVIMENTO
function teste(execute){
    if(!execute) return;
    let numeros = [[1, 2], [1, 3]];
    const [row, col] = numeros.find(e => e[0] === 1 && e[1] === 2);
}

/**
 * Verifica se a direcao em que se encontra a selecao de letras feita pelo utilizador
 * @param {*} fstRow posicao da linha onde se encontra a 1º letra da palavra em questão
 * @param {*} fstCol posicao da coluna onde se encontra a 1º letra da palavra em questão
 * @param {*} lstRow posicao da linha onde se encontra a ultima letra da palavra em questão
 * @param {*} lstCol posicao da coluna onde se encontra a ultima letra da palavra em questão
 * @returns -> h se horizontal | v se vertical | o1 se obliqua pra direita e o2 se pra esquerda
 */
function checkDir(fstRow, fstCol, lstRow, lstCol){
    if(fstRow === lstRow){
        return "h";
    }
    else if(fstCol === lstCol){
        return "v";
    }
    else{
        let tWidth = getTableW();
        let s = fstRow*tWidth+fstCol;
        let e = lstRow*tWidth+lstCol
        let t = lstRow - fstRow;
        let oPos = s + (t * tWidth) + t;
        if(fstCol<lstCol && e===oPos){
                return "o1";
        } 
        else{
            oPos = s + (t * tWidth) - t;
            if(e===oPos){
                return "o2";
            }
        }
    }
    return "undefined";
}

/**
 * Devolve tWidth dependendo do nivel selecionado
 * @returns -> Largura do tabuleiro
 */
function getTableW(){
    let tWidth = document.querySelector("#lvl-select").value;
    switch(tWidth){
        case "1": tWidth=6; break;
        case "2": tWidth=9; break;
        case "3": tWidth=12; break;
        default: tWidth=12; break;
    }
    return tWidth;
}

/**
 * Verifica se a letra em questão se encontra na selecao feita pelo utilizador
 * @param {*} start linha e coluna do primeiro elemento da selecao
 * @param {*} end linha e coluna do ultimo elemento da selecao
 * @param {*} posRow linha da letra em questao
 * @param {*} posCol coluna da letra em questao
 * @returns True se letra esta dentro da selecao, False se fora
 */
function isBetween(start, end, posRow, posCol){
    const [sRow, sCol] = start; //Comeco da palavra
    const [eRow, eCol] = end; //Fim da palavra
    let direction = checkDir(sRow, sCol, eRow, eCol);
    if(direction==="h"){
        if(posCol>sCol && posCol<eCol && sRow===posRow){
            return true;
        }
    }
    else if(direction==="v"){
        if(posRow>sRow && posRow<eRow && sCol===posCol){
            return true;
        }
    }else if(direction==="o1"){
        let cont = 1;
        for(let i= sRow+1; i<eRow; i++){
            if(sCol+cont===posCol && i===posRow){
                return true;
            }
            ++cont;
        }
    }
    else if(direction==="o2"){
        let cont = 1;
        for(let i= sRow+1; i<eRow; i++){
            if(sCol-cont===posCol && i===posRow){
                return true;
            }
            ++cont;
        }
    }
}

/**
 * Verifica se o index global do de um item do array tabuleiro, é maior que o de outro index
 * index global = pos de um array bidimensional num array unidimensional
 * @param {*} value1 linha e coluna do 1º item
 * @param {*} value2 linha e coluna do 2º item
 * @returns True se o 1º item é maior que o 2º, False se contraio
 */
function isHigher(value1, value2){
    const [aRow, aCol] = value1;
    const [bRow, bCol] = value2;
    return (aRow*getTableW()+aCol) > (bRow*getTableW()+bCol);
}

/**
 * Obtem palavras aleatorias e guardas num array global, words
 * Chama a funcao updateRwords para atualizar a lista de palavrasno painel Words
 * reseta a var que guarda os indexs globais: de letter-boxes que estao ocupadas por palavras
 * reseta a variavel unusedWords que guarda as palavras que nao foi possivel usar, das palavras no array words
 * Para cada elemento em words: 
 * ->gera randomicamente os valores que vao definir a direcao da palavra, sentido e posicao
 * ->dependendo da direcao da selecao, verifica se:
 * ->->a posicao onde a palavra vai ser spawnada n esta ocupada por outra palavra:
 * ->->->se não spawna a palavra no array do tabuleiro e adiciona as posicoes globais das letras da mesma ao array ocupiedBoxs
 * Se houverem palavras que não poderam ter sido usadas, elimina-as da lista de palavras a descobrir
 * @param {*} tWidth 
 * @param {*} table 
 */
function spawnWords(tWidth, table){
    words= randomWords(document.querySelector("#lvl-select").value, tWidth);
    updateRwords(words);
    let ocupiedBoxs = [];
    let unusedWords = [];
    //O block n funciona bem na horizontal
    words.forEach((word, index)=>{
        let success = false;
        let loopTimeout = 0;
        do {
            let wordCopy = word;
            success = false;
            let dir = Math.floor(Math.random()*4); 
            let startPosRow = Math.floor(Math.random()*(tWidth-1));  
            let startPosCol = Math.floor(Math.random()*(tWidth-1));
            let way = Math.floor(Math.random()*2); 
            let blocked = false; 

            if(way)
                wordCopy = invertString(wordCopy);

            if(dir == 0){
                if(startPosCol + word.length-1 <= tWidth-1){
                    for(let box of ocupiedBoxs){//0 é a row | 1 é a col
                        if((box[1]>=startPosCol && box[1]<=startPosCol + word.length) && box[0]===startPosRow){
                            blocked = true;
                            break;
                        } 
                    }
                    if(!blocked){
                        for(let y=0; y<word.length; y++){
                            table[startPosRow][startPosCol+y] = wordCopy[y].toLocaleLowerCase();
                            ocupiedBoxs.push([],[startPosRow, startPosCol+y]);
                        }
                        success = true;
                    }
                }
            }
            else if(dir == 1){
                if(startPosRow + word.length-1 <= tWidth-1){
                    for(let box of ocupiedBoxs){//0 é a row | 1 é a col
                        if((box[0]>=startPosRow && box[0]<= startPosRow + word.length) && box[1]===startPosCol){
                            blocked = true;
                            break;
                        } 
                    }
                    if(!blocked){
                        for(let y=0; y<word.length; y++){
                            table[startPosRow+y][startPosCol] = wordCopy[y].toLocaleLowerCase();
                            ocupiedBoxs.push([],[startPosRow+y, startPosCol]);
                        }
                        success = true;
                    }
                }
            }
            else if(dir == 2){
                if(startPosRow + word.length-1 <= tWidth-1 && startPosCol + word.length-1 <= tWidth-1){
                    for(let y=0; y<word.length; y++){
                        if(ocupiedBoxs.find(e => e[0] === startPosRow+y && e[1] === startPosCol+y) != undefined){
                            blocked = true;
                            break;
                        }
                    }
                    if(!blocked){
                        for(let y=0; y<word.length; y++){
                            table[startPosRow+y][startPosCol+y] = wordCopy[y].toLocaleLowerCase();
                            ocupiedBoxs.push([],[startPosRow+y, startPosCol+y]);
                        }
                        success = true;
                    }
                }
            }
            else{
                if(startPosRow + word.length-1 <= tWidth-1 && startPosCol - word.length-1 >= 0){
                    for(let y=0; y<word.length; y++){
                        if(ocupiedBoxs.find(e => e[0] === startPosRow+y && e[1] === startPosCol-y) != undefined){
                            blocked = true;
                            break;
                        }
                    }
                    if(!blocked){
                        for(let y=0; y<word.length; y++){
                            table[startPosRow+y][startPosCol-y] = wordCopy[y].toLocaleLowerCase();
                            ocupiedBoxs.push([],[startPosRow+y, startPosCol-y]);
                        }
                        success = true;
                    }
                }
            }
            ++loopTimeout;
        } while (success===false && loopTimeout<500);
        if(!success)
            unusedWords.push(index);
    })
    if(unusedWords.length>0){
        let wordsC = [];
        words.forEach((word, index)=>{
            let keep = true;
            for(let i of unusedWords){
                if(i===index){
                    keep = false;
                }
            }
            if(keep){
                wordsC.push(word);
            }
        });
        words = wordsC;
    }
}

/**
 * Verifica a direcao da selecao e dependendo da mesma constroi uma string com as letras entre a selecao, 
 * e adiciona a posicao das mesmas ao array tempHolder
 * Se a selecao for uma palvra da lista de palavras a diciona a palvra e as posicoes das suas letras aos arrays 
 * discoveredWords e discoveredPos, respetivamente
 * @param {*} start inicio da selecao de letras
 * @param {*} end fim da selecao de letras
 * @param {*} table array do tabuleiro
 * @returns True se a selecao é uma palavra valida (nao repetida), False se n
 */
function isValidWord(start, end, table){
    let holder="";
    const [sRow, sCol] = start; //Comeco da palavra
    const [eRow, eCol] = end; //Fim da palavra
    let direction = checkDir(sRow, sCol, eRow, eCol);
    let tempHolder = [];

    if(direction==="h"){
        for(let i=sCol; i<eCol+1; i++){
            holder+=table[sRow][i];
            tempHolder.push(sRow*getTableW()+i);
        }
    }
    else if(direction==="v"){
        for(let i=sRow; i<eRow+1; i++){
            holder+=table[i][sCol];
            tempHolder.push(i*getTableW()+sCol);
        }
    }
    else if(direction==="o1"){
        for(let i=0; i<(eCol-sCol)+1; i++){
            holder+=table[sRow+i][sCol+i];
            tempHolder.push((sRow+i)*getTableW()+sCol+i);
        }
    }
    else if(direction==="o2"){
        for(let i=0; i<(sCol-eCol)+1; i++){
            holder+=table[sRow+i][sCol-i];
            tempHolder.push((sRow+i)*getTableW()+sCol-i);
        }
    }
    else {holder = "undefined";}

    for(let word of words){
        word = word.toLowerCase();
        holder = holder.toLowerCase();
        for(let dWord of discoveredWords){
            if(holder === dWord){
                return false;
            }
        }
        if(word == holder || invertString(word) == holder ){
            if(invertString(word) == holder){
                holder = invertString(holder);
            }
            discoveredWords.push(holder);
            discoveredPos.push(...tempHolder);
            return true;
        }
    }
    return false;
}

/**
 * Verifica se a letra em questao faz parte de uma palavra descoberta
 * @param {*} row linha da letra
 * @param {*} col coluna da letra
 * @returns True se a letra faz parte de uma palvra descoberta, False s n
 */
function isDiscovered(row, col){
    let globPos = row*getTableW()+col;

    if(discoveredPos.length <= 0)
        return false;

    for(let pos of discoveredPos){
        if(pos === globPos){
            return true;
        }
    }
    return false;
}

/**
 * Atualiza as lista das palvras descobertas no panel Words, para que elas aparessam riscadas
 * @returns o array com as palavras descobertas
 */
function updateDisWords(){
    return discoveredWords;
}

/**
 * Inverte uma string
 * @param {*} str string original
 * @returns string invertida
 */
function invertString(str){
    var invertedString = "";
    for (var i = str.length - 1; i >= 0; i--) {
        invertedString += str[i];
    }
    return invertedString;
    //return str; //Anula a inversao PARA TESTES
}


export{
    updateDisWords,
    isDiscovered,
    isValidWord,
    isHigher,
    buildTable,
    checkDir,
    isBetween,
}