module.exports = app => {
    const lects = require("../controllers/lect.controller.js");
  
    var router = require("express").Router();
  
    router.post("/add", lects.create);

    router.get("/", lects.findByCourse);

    router.get('/search', lects.findByTitle);

    router.delete("/:id", lects.delete);
  
    app.use("/api/lects", router);
  };