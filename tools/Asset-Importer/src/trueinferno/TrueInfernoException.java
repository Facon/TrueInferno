package trueinferno;

public class TrueInfernoException extends Exception {
	private static final long serialVersionUID = 1L;

	public TrueInfernoException(String msg) {
		super(msg);
	}

	public TrueInfernoException(String msg, Exception e) {
		super(msg, e);
	}

}
