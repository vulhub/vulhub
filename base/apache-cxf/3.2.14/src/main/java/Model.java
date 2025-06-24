/**
 * @author ReaJason
 * @since 2025/5/10
 */
public class Model {
    private String text;
    private int count;
    private boolean you;

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }

    public String getText() {
        return text;
    }

    public void setText(String text) {
        this.text = text;
    }

    public boolean isYou() {
        return you;
    }

    public void setYou(boolean you) {
        this.you = you;
    }

    @Override
    public String toString() {
        return "Model{" + "count=" + count +
                ", text='" + text + '\'' +
                ", you=" + you +
                '}';
    }
}
