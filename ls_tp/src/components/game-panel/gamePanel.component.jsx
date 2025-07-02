import './gamePanel.css';
import React, {useState} from 'react';
import LetterBox from "./letter-box/letterBox.component"; 
import{
    buildTable,
    isHigher,
    isBetween,
    isValidWord,
    isDiscovered,
    updateDisWords
}from '../../functions/gameLogic';

let pressed = false;
let table = [];
let lvl;
function updateTable(){
    lvl = document.querySelector("#lvl-select").value
    table = buildTable();
}

export default function GamePanel(prop){
    const [firstLetterPos, setFirstLetterPos] = useState([undefined,undefined]);
    const [lastLetterPos, setLastLetterPos] = useState([undefined,undefined]);

    //Quando o botao to rato e primido numa letra:
    function handleDown(row, col){
        if(pressed){
            setLastLetterPos([row, col]);
            handleUp();
            pressed = false;
        }
        else{
            pressed = true;
            setFirstLetterPos([row, col]);   
        }
    }

    //Quando o rato passa por cima de uma letra
    function handleHover(row, col){
        if(pressed){
            setLastLetterPos([row, col]);
        }
    }

    //Quando o botao do rato é largado
    function handleUp(){
        if(pressed){
            let isValid = false;
            let start = isHigher(firstLetterPos, lastLetterPos) ? lastLetterPos : firstLetterPos;
            let end = isHigher(lastLetterPos, firstLetterPos) ? lastLetterPos : firstLetterPos;
            isValid = isValidWord(start, end, table);
            setLastLetterPos([undefined, undefined]);
            setFirstLetterPos([undefined, undefined]);
            if(isValid){
                prop.updateDiscWods(updateDisWords());
            }
        }
        pressed = false;
    }

    //Atualiza a classe do gamePanel para adaptar a grid ao tabuleiro //@!importREVER ESTE CODIGO
    function updateGameClass(){
        switch(lvl){
            case "1": return "level1"; break;
            case "2": return "level2"; break;
            case "3": return "level3"; break;
            default: return "level1"; break;
        }
    }

    return(
    <div id="game-panel" className={`${prop.hide ? "hide" : ""} ${updateGameClass()}`}>
        {table.map((elem,row)=>{return(elem.map((subElem, col)=>{return(
                <LetterBox 
                key={(row+1)*col} 
                //Handlers
                onDown={handleDown}
                onHover={handleHover}
                onUp={handleUp}
                //end---handlers

                //Funcoes utilidade
                isHigher={isHigher}
                isBetween={isBetween}
                isDiscovered={isDiscovered}

                //Indentificacao do componente
                row={row}
                col={col}

                //Variaveis de estado
                fLetter={firstLetterPos}
                lLetter={lastLetterPos}

                >
                    {//row*elem.length+col
                    subElem
                        
                    } 

                </LetterBox>
            );
        }));})}
    </div>
);}

export {updateTable as updateTable};