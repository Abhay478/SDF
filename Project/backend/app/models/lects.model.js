const mongoose = require("mongoose");

const Lects = mongoose.model(
  "Lect",
  new mongoose.Schema({
    course_id : String,
    title : String,
    link: String
  })
);

module.exports = Lects;