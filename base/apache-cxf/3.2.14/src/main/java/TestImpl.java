public class TestImpl implements Test {

    @Override
    public String test(Model model) {
        return model.toString();
    }
}