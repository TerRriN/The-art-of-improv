import React from "react";
import "../style/NavBar.css";

function MainBody() {
  return (
    <div className="navBarBody">
      <div className="navBarBtn firstBtn">
        First<div className="navBarBtnEffect"></div>
      </div>
      <div className="navBarBtn secondBtn">
        Second<div className="navBarBtnEffect"></div>
      </div>
      <div className="navBarBtn thirdBtn">
        Third<div className="navBarBtnEffect"></div>
      </div>
    </div>
  );
}

export default MainBody;
