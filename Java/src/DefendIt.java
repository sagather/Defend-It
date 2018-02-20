import java.math.BigInteger;
import java.util.*;
import java.util.regex.*;
import javax.crypto.*;
import javax.crypto.spec.PBEKeySpec;
import java.security.*;
import java.security.spec.InvalidKeySpecException;
import java.text.*;
import java.text.Normalizer.Form;

public class DefendIt
{
    private static Scanner kb = new Scanner(System.in);
    private static final Random RANDOM = new SecureRandom();
    private static final String ALPHABET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    private static final int ITERATIONS = 10000;
    private static final int KEY_LENGTH = 256;

    public static void main(String[] args){

        String name = getName();
        while(!checkName(name)){

            name = getName();

        }

        String password = getPassword();

        while(!checkPass(password)){

            password = getPassword();

        }

    }

    public static String getName()
    {
        System.out.print("Please enter a name (less than 50 characters long): ");
        return kb.nextLine();
    }

    public static boolean checkName(String name)
    {
        assert name != null || name != " " || name.length() > 50: "Invalid name, please try again \n" + getName();

        String s = "\uFE64" + name + "\uFE65";
        s = Normalizer.normalize(s, Form.NFKC);
        Pattern pattern = Pattern.compile("[<>]");
        Matcher matcher = pattern.matcher(s);
        try{
            matcher.find();
        }
        catch (IllegalFormatException e){
            System.out.println("Not a valid format for name.");
        }
        return true;
    }

    public static String getPassword()
    {
        System.out.print("Please enter a password that contains at least 10 characters and includes at least one upper case character, one lower case character, one digit, one punctuation mark: ");
        return kb.nextLine();
    }

    public static boolean checkPass(String pass)
    {
        String exp = "^((?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[!@#$%]).{10,})$";
        CharSequence input = pass;
        Pattern pattern = Pattern.compile(exp);
        Matcher matcher = pattern.matcher(input);
        if(matcher.matches())
            return true;
        return false;
    }

    public static String getSalt(int length)
    {
        StringBuilder val = new StringBuilder(length);
        for (int i = 0; i < length; i++)
            val.append(ALPHABET.charAt(RANDOM.nextInt(ALPHABET.length())));

        return new String(val);
    }

    public static byte[] hash(char[] pass, byte[] salt)
    {
        PBEKeySpec key = new PBEKeySpec(pass, salt, ITERATIONS, KEY_LENGTH);
        Arrays.fill(pass, Character.MIN_VALUE);
        try
        {
            SecretKeyFactory skf = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA1");
            return skf.generateSecret(key).getEncoded();
        }
        catch (NoSuchAlgorithmException | InvalidKeySpecException e)
        {
            throw new AssertionError("Error while hashing a password: " + e.getMessage(), e);
        }
        finally
        {
            key.clearPassword();
        }
    }

    public static String generateSecurePass(String pass, String salt)
    {
        byte[] securePass = hash(pass.toCharArray(), salt.getBytes());
        return Base64.getEncoder().encodeToString(securePass);
    }

    public static boolean verifyPass(String providedPass, String securedPass, String salt)
    {
        String newPass = generateSecurePass(providedPass, salt);
        if(newPass.equalsIgnoreCase(securedPass))
            return true;
        return false;
    }

    public static BigInteger getInt(){

        System.out.println("Please enter an int between −2,147,483,648 and 2,147,483,647");

        BigInteger int1 = BigInteger.ZERO;

        try{
            String input = kb.nextLine();
            int1 = new BigInteger(input);
        }
        catch (NumberFormatException e){
            System.out.println("Check your int... were there any letters or symbols in it?");
        }

        return int1;

    }

    public static boolean checkInt(BigInteger int1){

        if(int1.compareTo(new BigInteger("-2147483648")) < 0 || int1.compareTo(new BigInteger("2147483648")) > 0){
            System.out.println("Invalid range\n");
            return false;
        }

        return true;

    }

}

