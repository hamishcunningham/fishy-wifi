package harvest

import grails.test.mixin.*
import spock.lang.*

@TestFor(CropController)
@Mock(Crop)
class CropControllerSpec extends Specification {

    def populateValidParams(params) {
        assert params != null

        // TODO: Populate valid properties like...
        //params["name"] = 'someValidName'
        assert false, "TODO: Provide a populateValidParams() implementation for this generated test suite"
    }

    void "Test the index action returns the correct model"() {

        when:"The index action is executed"
            controller.index()

        then:"The model is correct"
            !model.cropList
            model.cropCount == 0
    }

    void "Test the create action returns the correct model"() {
        when:"The create action is executed"
            controller.create()

        then:"The model is correctly created"
            model.crop!= null
    }

    void "Test the save action correctly persists an instance"() {

        when:"The save action is executed with an invalid instance"
            request.contentType = FORM_CONTENT_TYPE
            request.method = 'POST'
            def crop = new Crop()
            crop.validate()
            controller.save(crop)

        then:"The create view is rendered again with the correct model"
            model.crop!= null
            view == 'create'

        when:"The save action is executed with a valid instance"
            response.reset()
            populateValidParams(params)
            crop = new Crop(params)

            controller.save(crop)

        then:"A redirect is issued to the show action"
            response.redirectedUrl == '/crop/show/1'
            controller.flash.message != null
            Crop.count() == 1
    }

    void "Test that the show action returns the correct model"() {
        when:"The show action is executed with a null domain"
            controller.show(null)

        then:"A 404 error is returned"
            response.status == 404

        when:"A domain instance is passed to the show action"
            populateValidParams(params)
            def crop = new Crop(params)
            controller.show(crop)

        then:"A model is populated containing the domain instance"
            model.crop == crop
    }

    void "Test that the edit action returns the correct model"() {
        when:"The edit action is executed with a null domain"
            controller.edit(null)

        then:"A 404 error is returned"
            response.status == 404

        when:"A domain instance is passed to the edit action"
            populateValidParams(params)
            def crop = new Crop(params)
            controller.edit(crop)

        then:"A model is populated containing the domain instance"
            model.crop == crop
    }

    void "Test the update action performs an update on a valid domain instance"() {
        when:"Update is called for a domain instance that doesn't exist"
            request.contentType = FORM_CONTENT_TYPE
            request.method = 'PUT'
            controller.update(null)

        then:"A 404 error is returned"
            response.redirectedUrl == '/crop/index'
            flash.message != null

        when:"An invalid domain instance is passed to the update action"
            response.reset()
            def crop = new Crop()
            crop.validate()
            controller.update(crop)

        then:"The edit view is rendered again with the invalid instance"
            view == 'edit'
            model.crop == crop

        when:"A valid domain instance is passed to the update action"
            response.reset()
            populateValidParams(params)
            crop = new Crop(params).save(flush: true)
            controller.update(crop)

        then:"A redirect is issued to the show action"
            crop != null
            response.redirectedUrl == "/crop/show/$crop.id"
            flash.message != null
    }

    void "Test that the delete action deletes an instance if it exists"() {
        when:"The delete action is called for a null instance"
            request.contentType = FORM_CONTENT_TYPE
            request.method = 'DELETE'
            controller.delete(null)

        then:"A 404 is returned"
            response.redirectedUrl == '/crop/index'
            flash.message != null

        when:"A domain instance is created"
            response.reset()
            populateValidParams(params)
            def crop = new Crop(params).save(flush: true)

        then:"It exists"
            Crop.count() == 1

        when:"The domain instance is passed to the delete action"
            controller.delete(crop)

        then:"The instance is deleted"
            Crop.count() == 0
            response.redirectedUrl == '/crop/index'
            flash.message != null
    }
}
