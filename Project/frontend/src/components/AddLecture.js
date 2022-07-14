import React, { useRef, useState } from "react";
import { useNavigate, useParams } from "react-router-dom";
import LectService from "../services/lect.service";
import Form from "react-validation/build/form";
import Input from "react-validation/build/input";


const AddLect = () => {
    const course = useParams();
    console.log(course);
    const course_id = course.course_id;
    console.log(course_id);
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
  const initialLectState = {
    course_id:course_id,
    title:"",
    link:""
  };
  const [lect, setLect] = useState(initialLectState);
  const [submitted, setSubmitted] = useState(false);

  const handleInputChange = event => {
    const { name, value } = event.target;
    setLect({ ...lect, [name]: value });
  };

  const saveLect = () => {
    var data = {
      title: lect.title,
      course_id: lect.course_id,
      link: lect.link
    };

    LectService.createLect(data)
      .then(response => {
        setLect({
          title: response.data.title,
          course_id: response.data.course_id,
          link: response.data.link
        });
        setSubmitted(true);
        console.log(response.data);
      })
      .catch(e => {
        console.log(e);
      });

      setSubmitted(true);
  };

  const newLect = () => {
    setLect(initialLectState);
    setSubmitted(false);
  };

  return (
    <div className="submit-form">
      {submitted ? (
        <div>
          <h4>You submitted successfully!</h4>
          <button className="m-3 btn btn-sm btn-success" onClick={newLect}>
            Add
          </button>
          
          <button className="m-3 btn btn-sm btn-success" onClick={() => {navigate('/courses')}}>
            Finish
          </button>
          
        </div>
      ) : (
        <Form onSubmit={saveLect} ref={form}>
            <h1>{course_id}</h1>
          <div className="form-group">
            <label htmlFor="title">Title</label>
            <Input
              type="text"
              className="form-control"
              id="title"
              required
              value={lect.title}
              onChange={handleInputChange}
              name="title"
              validations={[required]}
            />
          </div>

          <div className="form-group">
            <label htmlFor="link">Link</label>
            <Input
              type="url"
              className="form-control"
              id="link"
              required
              value={lect.link}
              onChange={handleInputChange}
              name="link"
              validations={[required]}
            />
          </div>

          <button type='submit' className="btn btn-success">
            Submit
          </button>
        </Form>
      )}
    </div>
  );
};

export default AddLect;
