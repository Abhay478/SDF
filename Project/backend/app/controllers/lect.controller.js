const db = require('../models');
const Lect = db.lects;

exports.create = (req, res) => {
    const lect = new Lect({
        course_id : req.body.course_id,
        title : req.body.title,
        link : req.body.link
    })
    
    lect
    .save(lect)
    .then(data => {
      res.send(data);
      console.log("Lecture added.");
    })
    .catch(err => {
      res.status(500).send({
        message:err.message
      });
    });
};

exports.findByCourse = (req, res) => {
    const course_id = req.query.course_id;
    
    var condition = course_id ? {course_id :  { $regex: new RegExp(course_id), $options: "i" }} : {};
  
    Lect.find(condition)
      .then(data => {
        res.send(data);
      })
      .catch(err => {
        res.status(500).send({
          message:err.message
        });
      });
};

exports.delete = (req, res) => {
  const id = req.params.id;
  Lect.deleteOne({_id : id})
  .then(data => console.log(data))
  .catch(err => {
    res.status(500).send({
      message:
        err.message || "Some error occurred while retrieving lectures."
    });
  });
}

exports.findByTitle = (req, res) => {
  const title = req.query.title;
  Lect.find({title : title})
  .then(data => res.send(data))
  .catch(err => {
    res.status(500).send({
      message:
        err.message || "Some error occurred while retrieving lectures."
    });
  });
}

// exports.update = (req, res) => {
//   const id = req.params.id;
//   Sub.updateOne({_id : id}, {$set : {score : req.body.score, grade : req.body.grade}})
//   .then(data => res.send(data))
//   .catch(err => {
//     res.status(500).send({
//       message:
//         err.message || "Some error occurred while updating courses."
//     });
//   });
// }

