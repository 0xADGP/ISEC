import React, {useState} from "react";
import './wordsPanel.css';
import { allWords } from "../../../functions/words"; 

let words;
function updateRwords(_words){
    words = _words;
}

export default function WordPanel(prop){
    function isDiscovered(thisWord){
        for(let word of prop.discWords){
            let _word = word.toUpperCase();
            thisWord = thisWord.toUpperCase();
            if(_word === thisWord){
                return true;
            }
        }
        return false;
    }
    //const [words, setWords] = useState([...allWords]);
    if (!prop.gameStarted){
        words = [...allWords];
    }
    return(
        <div className="words-panel">
            <h1>PALAVRAS</h1>
            <div className="words-panel padding">
                {words.map((item, index)=>{return <span className={`${isDiscovered(item)?"crossed":""}`} key={index}>{item}</span>})}
            </div>
        </div>
);}

export {updateRwords as updateRwords};