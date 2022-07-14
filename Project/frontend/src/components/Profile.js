import React from "react";
import AuthService from "../services/auth.service";

const Profile = () => {
  const currentUser = AuthService.getCurrentUser();

  return (
    <div className="container">
      <header className="jumbotron">
        <h1>
          <strong>{currentUser.username}'s</strong> Account Details
        </h1>
      <h3>
      <p>
          <strong>Authority:</strong> {currentUser.authority}
      </p>
      <p>
        <strong>Name:</strong> {currentUser.name}
      </p>
      <p>
        <strong>Email:</strong> {currentUser.email}
      </p>
      <p>
        <strong>Token:</strong> {currentUser.accessToken.substring(0, 10)} ...{" "}
        {currentUser.accessToken.substr(currentUser.accessToken.length - 10)}
      </p>
      <p>
        <strong>Id:</strong> {currentUser.id}
      </p>
      </h3>
      </header>
    </div>
  );
};

export default Profile;
