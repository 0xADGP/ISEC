import { hover } from "@testing-library/user-event/dist/hover";
import React, { useEffect} from "react";
import './letterBox.css';

export default function LetterBox(prop){
    let hovered = "";
    let start = prop.isHigher(prop.fLetter, prop.lLetter) ? prop.lLetter : prop.fLetter;
    let end = prop.isHigher(prop.lLetter, prop.fLetter) ? prop.lLetter : prop.fLetter;

    const [fRow, fCol] = prop.fLetter;
    const [lRow, lCol] = prop.lLetter;

    if((prop.row===fRow && prop.col===fCol) || (prop.row===lRow && prop.col===lCol)){
        hovered = "hovered";
    }   
    else if(prop.isBetween(start, end, prop.row, prop.col)){
        hovered = "hovered";
    }
    else if(prop.isDiscovered(prop.row, prop.col)){
        hovered = "hovered";
    }

    return(
    <div className={`letter-box ${hovered}`} 
        onMouseDown={() => {
            prop.onDown(prop.row, prop.col);
        }}
        onMouseOver={() => {
            prop.onHover(prop.row, prop.col);
        }}
        onMouseUp={() => {
            prop.onUp();
        }}
    >
        {prop.children}
    </div>
);}