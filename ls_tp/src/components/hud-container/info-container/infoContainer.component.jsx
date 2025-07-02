import './infoContainer.css';
import React from "react";

export default function InfoContainer(prop){return(
    <div id="info-container">
        <h1>INFORMACOES</h1>
        {prop.children}
        <button onClick={prop.changeGameStatus} id='bt_play' className='button'>{prop.gameStarted ? "PARAR" : "JOGAR"}</button>
    </div>
);}