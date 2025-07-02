import './assets/styles/App.css';
import React, {useState, useEffect} from 'react';
import updateTableArray from './functions/gameLogic'; 
import{
  Header,
  HudContainer,
  GamePanel,
  Footer,
  GameOverPanel
} from './components/'

import { updateTable } from './components/game-panel/gamePanel.component';
let points = 0;
let gameover = false;
let timerId;

function App(){
  const [gameStarted, setGameStarted] = useState(false);
  const [discWords, setDiscWods] = useState([]);
  const [timer, setTimer] = useState([60]);

  function changeGameStatus(){
    updateTable();
    setDiscWods([]);
    points = 0;
    if(timer <= 0){
      gameover = true;
      clearInterval(timerId);
    }
    setTimer(60);
    gameStarted ? setGameStarted(false) : setGameStarted(true);
  }

  useEffect(() => {
        timerId = setInterval(() => {
            setTimer(prevState => prevState-1);
        }, 1000);
        return () =>{
            if(!gameStarted){
                clearInterval(timerId);
                setTimer(60)
            }
            
        }
  }, []);

  if(timer == 0 && gameStarted){
    changeGameStatus();
  }


  function updateDiscWords(arrWods){
    points+=10;
    setDiscWods([...arrWods]);
  }

  function updateGameover(){
    gameover = false;
    setTimer(60);
  }

  return (
  <div id='main'>
    <GameOverPanel hide={!gameover}
      updateGameover={updateGameover}/>
    <Header/>
    <div id='body'>
      <div className='game-container'>
        <GamePanel hide={!gameStarted} updateDiscWods={updateDiscWords}/>
        <HudContainer 
          changeGameStatus={changeGameStatus} 
          gameStarted={gameStarted}
          discWords={discWords}
          points={points}
          timer={timer}
        />
      </div>
      <div></div>
    </div>
    <Footer/>
  </div>
);}

export default App;
