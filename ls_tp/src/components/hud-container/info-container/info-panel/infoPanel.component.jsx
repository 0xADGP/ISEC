import React from "react";
import './infoPanel.css';
import { addWord } from "../../../../functions/words"; 

export default function InfoPanel(props){return(
    <div id="info-panel" className={props.hide ? "hide" : ""}>
        <span>
            <label className="label">Nivel:</label>
            <select defaultValue="2" name="" id="lvl-select" className="selection">
                <option value="1">Facil</option>
                <option value="2">Medio</option>
                <option value="3">Dificil</option>
            </select>
        </span>
        <label onClick={()=>addWord()}>+palavras</label>
    </div>
);}