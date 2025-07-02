
let allWords = [
    "React", "Linguagens", "Bootstrap", "Vue", "Loop", "Function",
    "Joao", "Balao", "Colchao", "Banhao", "Professorzao", "Roncar",
    "Pinar", "Chutar", "Rodalar", "Pedalar", "Primeiratar", "Cao", "Rat"
];

export default function randomWords(lvl, tableW){
    let words = [];
    let random, cont=0, nLoops;

    switch(lvl){
        case "1": nLoops = 3; break;
        case "2": nLoops = 6; break;
        case "3": nLoops = 9; break; //9
    }
    do{
        random =Math.floor(Math.random()*allWords.length);
        let repeated = false;
        for(let word of words){
            if(word == allWords[random]){
                repeated = true;
            }
        }
        if(!repeated && allWords[random].length<tableW){
            cont++;
            words.push(allWords[random]);
        }
    }while(cont<nLoops)
    return words;
}

function addWord(){
    allWords.push(prompt("Introduza uma word", "Manjericão"));
}

export {addWord};
export {allWords as allWords};