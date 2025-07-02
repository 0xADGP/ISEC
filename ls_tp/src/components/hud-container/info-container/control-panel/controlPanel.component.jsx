import React from "react";
import './controlPanel.css';

export default function ControlPanel(props){return(
    <div id="control-panel" className={props.hide ? "hide" : ""}>
        <div>
            <label className="label">Tempo:</label>
            <span className="display">{props.timer}</span>
        </div>
        <div>
            <label className="label">Pontos:</label>
            <span className="display">{props.points}</span>
        </div>
    </div>
);}