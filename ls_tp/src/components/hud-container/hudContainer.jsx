import React from "react";
import './hudContainer.css'
import InfoContainer from "./info-container/infoContainer.component";
import InfoPanel from "./info-container/info-panel/infoPanel.component";
import WordPanel from "./words-panel/wordsPanel.component";
import ControlPanel from "./info-container/control-panel/controlPanel.component";

export default function HudContainer(prop){return(
    <div id="hud-container" className={prop.gameStarted ? "gameStarted" : ""}>
        <InfoContainer changeGameStatus={prop.changeGameStatus} gameStarted={prop.gameStarted}>
            <InfoPanel hide={prop.gameStarted}/>
            <ControlPanel hide={!prop.gameStarted} points={prop.points} timer={prop.timer}/>
        </InfoContainer>
        <hr/>
        <WordPanel discWords={prop.discWords} gameStarted={prop.gameStarted}/>
    </div>
);}