import React from "react";
import "../style/MainBody.css";
import Palette from "./Palette";
import NavBar from "./NavBar";

function MainBody() {
  return (
    <div className="mainBody">
      <NavBar />
      <Palette />
    </div>
  );
}

export default MainBody;
