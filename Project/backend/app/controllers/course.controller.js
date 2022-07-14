const db = require("../models");
const Course = db.courses;

// Retrieve all Courses from the database.
exports.findAll = (req, res) => {
  const title = req.query.title;
  var condition = title ? { title: { $regex: new RegExp(title), $options: "i" } } : {};

  Course.find(condition)
    .then(data => {
      res.send(data);
    })
    .catch(err => {
      res.status(500).send({
        message:err.message
      });
    });
};

// Create and Save a new Course
exports.create = (req, res) => {
  // Validate request
  if (!req.body.title) {
    res.status(400).send({ message: "Content cannot be empty." });
    return;
  }

  // Create a Course
  const course = new Course({
    title: req.body.title,
    description: req.body.description,
    instructor: req.body.instructor,
    published: req.body.published ? req.body.published : false
  });

  // Save Course in the database
  course
    .save(course)
    .then(data => {
      res.send(data);
    })
    .catch(err => {
      res.status(500).send({
        message:err.message
      });
    });
};

// Find a single Course with an id
exports.findOne = (req, res) => {
  const id = req.params.id;

  Course.findById(id)
    .then(data => {
      if (!data)
        res.status(404).send({ message: "No Course with id " + id });
      else res.send(data);
    })
    .catch(err => {
      res
        .status(500)
        .send({ message: "Error getting Course with id=" + id });
    });
};

// Delete a Course with the specified id in the request
exports.delete = (req, res) => {
  const id = req.params.id;

  Course.findByIdAndRemove(id, { useFindAndModify: false })
    .then(data => {
      if (!data) {
        res.status(404).send({
          message: "Cannot delete."
        });
      } else {
        res.send({
          message: "Course deleted."
        });
      }
    })
    .catch(err => {
      res.status(500).send({
        message: "Cannot delete."
      });
    });
};

// Update a Course by the id in the request
exports.update = (req, res) => {
  if (!req.body) {
    return res.status(400).send({
      message: "Data to update can not be empty!"
    });
  }

  const id = req.params.id;

  Course.findByIdAndUpdate(id, req.body, { useFindAndModify: false })
    .then(data => {
      if (!data) {
        res.status(404).send({
          message: "Cannot update."
        });
      } else res.send({ message: "Course updated." });
    })
    .catch(err => {
      res.status(500).send({
        message: "Cannot update."
      });
    });
};

// Delete all Courses from the database.
exports.deleteAll = (req, res) => {
  Course.deleteMany({})
    .then(data => {
      res.send({
        message: "Wiped."
      });
    })
    .catch(err => {
      res.status(500).send({
        message:err.message
      });
    });
};

// Find all published Courses
exports.findAllPublished = (req, res) => {
  Course.find({ published: true })
    .then(data => {
      res.send(data);
    })
    .catch(err => {
      res.status(500).send({
        message:err.message
      });
    });
};

