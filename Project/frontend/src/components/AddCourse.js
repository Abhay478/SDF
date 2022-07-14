import React, { useRef, useState } from "react";
import { useNavigate } from "react-router-dom";
import CourseService from "../services/course.service";
import Form from "react-validation/build/form";
import Input from "react-validation/build/input";


const AddCourse = () => {
  const form = useRef();
  const required = (value) => {
    if (!value) {
      return (
        <div className="alert alert-danger" role="alert">
          This field is required.
        </div>
      );
    }
  };
  const navigate = useNavigate();
  const initialCourseState = {
    id: null,
    title: "",
    description: "",
    instructor: "",
    published: false
  };
  const [course, setCourse] = useState(initialCourseState);
  const [submitted, setSubmitted] = useState(false);

  const handleInputChange = event => {
    const { name, value } = event.target;
    setCourse({ ...course, [name]: value });
  };

  const saveCourse = () => {
    var data = {
      title: course.title,
      description: course.description,
      instructor: course.instructor
    };

    CourseService.createCourse(data)
      .then(response => {
        setCourse({
          id: response.data.id,
          title: response.data.title,
          description: response.data.description,
          instructor: response.data.instructor,
          published: response.data.published
        });
        setSubmitted(true);
        console.log(response.data);
      })
      .catch(e => {
        console.log(e);
      });
  };

  const newCourse = () => {
    setCourse(initialCourseState);
    setSubmitted(false);
  };

  return (
    <div className="submit-form">
      {submitted ? (
        <div>
          <h4>You submitted successfully!</h4>
          <button className="m-3 btn btn-sm btn-success" onClick={newCourse}>
            Add
          </button>
          
          <button className="m-3 btn btn-sm btn-success" onClick={() => {navigate('/courses')}}>
            Finish
          </button>
          
        </div>
      ) : (
        <Form onSubmit={saveCourse} ref={form}>
          <div className="form-group">
            <label htmlFor="title">Title</label>
            <Input
              type="text"
              className="form-control"
              id="title"
              required
              value={course.title}
              onChange={handleInputChange}
              name="title"
              validations={[required]}
            />
          </div>

          <div className="form-group">
            <label htmlFor="description">Description</label>
            <Input
              type="text"
              className="form-control"
              id="description"
              required
              value={course.description}
              onChange={handleInputChange}
              name="description"
              validations={[required]}
            />
          </div>

          <div className="form-group">
            <label htmlFor="instructor">Instructor</label>
            <Input
              type="text"
              className="form-control"
              id="instructor"
              required
              value={course.instructor}
              onChange={handleInputChange}
              name="instructor"
              validations={[required]}
            />
          </div>

          <button className="btn btn-success">
            Submit
          </button>
        </Form>
      )}
    </div>
  );
};

export default AddCourse;
