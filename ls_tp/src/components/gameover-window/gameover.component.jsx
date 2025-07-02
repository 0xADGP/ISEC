import './gameover.css';

export default function GameOverPanel(prop){return(
    <div id='gameover' className={`${prop.hide ? "hide" : ""}`}>
        <div id='gameover-panel'>
            <p>Perdeu por tempo!</p>
            <div className='close' onClick={()=>{prop.updateGameover()}}>Fechar</div>
        </div>
    </div>
);}
