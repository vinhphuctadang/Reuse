import java.lang.reflect.Constructor;

public class Homie {
		
	
	public Foo getAFoo (String className) throws Exception{
		Foo foo = null;
		
		Class<?> aClass = Class.forName(className);
    	Constructor<?> constuctors = aClass.getConstructor(Object.class);
    	Object caller = Homie.this;
    	foo = (Foo) constuctors.newInstance (caller);
		return foo;
	}
	
	public static void main (String args[]) {
		
		Homie homie = new Homie ();
		try {
			Foo foo = homie.getAFoo("FooInherit");
			System.out.print(foo.getFoo());
		} catch (Exception e) {
			
		}
	}
}
