package harvest


import grails.test.mixin.TestFor
import spock.lang.Specification

/**
 * See the API for {@link grails.test.mixin.web.ControllerUnitTestMixin} for usage instructions
 */
@TestFor(ProfileCompletionInterceptor)
class ProfileCompletionInterceptorSpec extends Specification {

    def setup() {
    }

    def cleanup() {

    }

    void "Test profileCompletion interceptor matching"() {
        when:"A request matches the interceptor"
            withRequest(controller:"profileCompletion")

        then:"The interceptor does match"
            interceptor.doesMatch()
    }
}
