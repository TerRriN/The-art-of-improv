import React from "react";
import "../style/Menu.css";
import Palette from "./Palette";
import ormet1 from "../images/ormet1.png";
import sjuk1 from "../images/sjuk1.png";

function Menu() {
  return (
    <div className="main">
      <div className="navBar"></div>
      <div className="imgBody">
        <img className="imgPreview" src={ormet1} />
        <img className="imgPreview" src={sjuk1} />
        <img className="imgPreview" src={ormet1} />
        <img className="imgPreview" src={sjuk1} />
      </div>
      <Palette />
    </div>
  );
}

export default Menu;
